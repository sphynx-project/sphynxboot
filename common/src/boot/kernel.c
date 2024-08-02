/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <boot/kernel.h>
#include <data/elf.h>
#include <utils/wchar.h>
#include <lib/print.h>
#include <lib/string.h>
#include <lib/alloc.h>
#include <fs/sfs.h>

void map_kernel() {
}


void load_kernel(char *path)
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

    printf(" - Loading %s\n", path);
    map_kernel();

    CHAR16 *path_wide = malloc(strlen(kernel_path) * sizeof(CHAR16) + 2);
    utf8_char_to_wchar(kernel_path, path_wide);

    SimpleFile kernel = sfs_open(path_wide);
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

    sfs_read(&kernel, &*buffer);
    if (EFI_ERROR(kernel.status))
    {
        printf(" - Error: Failed to read kernel data: %d\n", kernel.status);
        for (;;)
            ;
        ;
    }

    elf_exec_handle *data = load_elf(buffer);
    if (data == NULL)
    {
        printf(" - Error: Failed to load kernel: Invalid ELF file\n");
        sfs_close(&kernel);
        free(buffer);
        free(path_wide);
        for (;;)
            ;
    }

    sfs_close(&kernel);
    free(buffer);
    free(path_wide);


    printf("%s: 0x%llx", path, data->entry_point);

    // TODO: Setup the env for the kernel and pass shit based on protocol
    systemTable->BootServices->ExitBootServices(imageHandle, 0);
    void (*entry)(void) = (void (*)(void))(uintptr_t)data->entry_point;
    entry();
}