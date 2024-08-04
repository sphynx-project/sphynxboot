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
#include <common.h>

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

    file_t ramfs_file = {0};

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

    framebuffer_t fb = load_framebuffer();
    if (fb.address == 0) {
        printf(" - Error: Failed to initialize framebuffer: Address is 0\n");
        for (;;)
            ;
    }

    EFI_UINTN memory_map_size = 0;
    EFI_MEMORY_DESCRIPTOR *memory_map = NULL;
    EFI_UINTN map_key;
    EFI_UINTN descriptor_size;
    uint32_t descriptor_version;

    EFI_STATUS status = systemTable->BootServices->GetMemoryMap(&memory_map_size, memory_map, &map_key, &descriptor_size, &descriptor_version);

    memory_map_size += 2 * descriptor_size;
    memory_map = (EFI_MEMORY_DESCRIPTOR *)malloc(memory_map_size);
    if (memory_map == NULL) {
        printf(" - Error: Failed to allocate memory for memory map\n");
        for (;;)
            ;
    }

    status = systemTable->BootServices->GetMemoryMap(&memory_map_size, memory_map, &map_key, &descriptor_size, &descriptor_version);
    if (EFI_ERROR(status)) {
        printf(" - Error: Failed to get memory map: %d\n", status);
        free(memory_map);
        for (;;)
            ;
    }

    EFI_UINTN num_entries = memory_map_size / descriptor_size;
    memory_map_t *boot_memory_map = (memory_map_t *)malloc(sizeof(memory_map_t) + num_entries * sizeof(memory_region_t));
    if (boot_memory_map == NULL) {
        printf(" - Error: Failed to allocate memory for boot memory map\n");
        free(memory_map);
        for (;;)
            ;
    }

    boot_memory_map->region_count = num_entries;
    EFI_MEMORY_DESCRIPTOR *desc = memory_map;
    for (EFI_UINTN i = 0; i < num_entries; ++i) {
        memory_region_t *region = &boot_memory_map->regions[i];
        region->base_address = desc->PhysicalStart;
        region->length = desc->NumberOfPages * 4096;
        region->type = desc->Type;

        desc = (EFI_MEMORY_DESCRIPTOR *)((uint8_t *)desc + descriptor_size);
    }

    info_t info = {
        .name = "sphynxboot"
    };

    boot_t boot_data = {
        .framebuffer = &fb,
        .info = &info,
        .memory_map = boot_memory_map
    };

    if (ramfs_file.size == 0) {
        boot_data.ramfs = NULL;
    } else {
        boot_data.ramfs = &ramfs_file;
    }

    systemTable->BootServices->ExitBootServices(imageHandle, map_key);

    typedef void (*entry_func_t)(boot_t*) __attribute__((sysv_abi));
    entry_func_t entry;
    entry = (entry_func_t)(uintptr_t)data->entry_point;
    entry(&boot_data);

    free(memory_map);
    free(boot_memory_map);
}
