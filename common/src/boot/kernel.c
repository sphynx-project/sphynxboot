/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/
#include <boot/kernel.h>
#include <boot/framebuffer.h>
#include <data/elf.h>
#include <utils/wchar.h>
#include <lib/print.h>
#include <lib/string.h>
#include <lib/alloc.h>
#include <fs/sfs.h>

void load_kernel(char *path, char *ramfs_path)
{
    char kernel_path[256];
    char *p;

    strncpy(kernel_path, path, sizeof(kernel_path) - 1);
    kernel_path[sizeof(kernel_path) - 1] = '\0';

    for (p = kernel_path; *p; ++p) {
        if (*p == '/') {
            *p = '\\';
        }
    }

    CHAR16 *kernel_path_wide = malloc(strlen(kernel_path) * sizeof(CHAR16) + 2);
    utf8_char_to_wchar(kernel_path, kernel_path_wide);

    SimpleFile kernel = sfs_open(kernel_path_wide);
    if (EFI_ERROR(kernel.status))
    {
        printf(" - Error: Failed to open \"%s\": File not found\n", path);
        for (;;)
            ;
    }

    char *buffer = malloc(kernel.info.PhysicalSize);
    if (buffer == NULL)
    {
        printf(" - Error: Failed to allocate memory for kernel data buffer\n");
        for (;;)
            ;
    }

    sfs_read(&kernel, buffer);
    if (EFI_ERROR(kernel.status))
    {
        printf(" - Error: Failed to read kernel data: %d\n", kernel.status);
        sfs_close(&kernel);
        free(buffer);
        free(kernel_path_wide);
        for (;;)
            ;
    }

    elf_exec_handle *data = load_elf(buffer);
    if (data == NULL)
    {
        printf(" - Error: Failed to load kernel: Invalid ELF file\n");
        sfs_close(&kernel);
        free(buffer);
        free(kernel_path_wide);
        for (;;)
            ;
    }

    sfs_close(&kernel);

    // RAMFS processing
    file_t ramfs_file = {0}; // Initialize to zero

    if (ramfs_path != NULL && ramfs_path[0] != '\0')
    {
        char ramfs_path_buffer[256];
        char *q;

        strncpy(ramfs_path_buffer, ramfs_path, sizeof(ramfs_path_buffer) - 1);
        ramfs_path_buffer[sizeof(ramfs_path_buffer) - 1] = '\0';

        for (q = ramfs_path_buffer; *q; ++q) {
            if (*q == '/') {
                *q = '\\';
            }
        }

        CHAR16 *ramfs_path_wide = malloc(strlen(ramfs_path_buffer) * sizeof(CHAR16) + 2);
        utf8_char_to_wchar(ramfs_path_buffer, ramfs_path_wide);

        SimpleFile ramfs = sfs_open(ramfs_path_wide);
        if (EFI_ERROR(ramfs.status))
        {
            printf(" - Error: Failed to open RAMFS \"%s\": File not found\n", ramfs_path);
            free(ramfs_path_wide);
        }
        else
        {
            char *ramfs_buffer = malloc(ramfs.info.PhysicalSize);
            if (ramfs_buffer == NULL)
            {
                printf(" - Error: Failed to allocate memory for RAMFS data buffer\n");
                sfs_close(&ramfs);
                free(ramfs_path_wide);
                for (;;)
                    ;
            }

            sfs_read(&ramfs, ramfs_buffer);
            if (EFI_ERROR(ramfs.status))
            {
                printf(" - Error: Failed to read RAMFS data: %d\n", ramfs.status);
                sfs_close(&ramfs);
                free(ramfs_buffer);
                free(ramfs_path_wide);
                for (;;)
                    ;
            }

            printf("RAMFS contents: %.*s\n", ramfs.info.FileSize, ramfs_buffer);
            printf("RAMFS Size: %d\n", ramfs.info.FileSize);

            ramfs_file.address = (void *)ramfs_buffer;
            ramfs_file.size = ramfs.info.FileSize;

            sfs_close(&ramfs);
            free(ramfs_path_wide);
        }
    }

    // The framebuffer setup
    framebuffer_t fb = load_framebuffer();
    if (fb.address == 0) {
        printf(" - Error: Failed to initialize framebuffer: Address is 0\n");
        for (;;)
            ;
    }

    info_t info = {
        .name = "sphynxboot"
    };

    boot_t boot_data = {
        .framebuffer = &fb,
        .info = &info
    };

    if(ramfs_file.size == 0) {
        boot_data.ramfs = NULL;
    } else {
        boot_data.ramfs = &ramfs_file;
    }

    systemTable->BootServices->ExitBootServices(imageHandle, 0);

    typedef void (*entry_func_t)(boot_t*) __attribute__((sysv_abi));
    entry_func_t entry;
    entry = (entry_func_t)(uintptr_t)data->entry_point;
    entry(&boot_data);
}
