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

void load_kernel()
{
    char *kernel_path = "kernel.elf";

    CHAR16 *path_wide = malloc(strlen(kernel_path) * sizeof(CHAR16) + 2);
    utf8_char_to_wchar(kernel_path, path_wide);

    SimpleFile kernel = sfs_open(path_wide);
    if (EFI_ERROR(kernel.status))
    {
        printf("ERROR: Failed to open kernel path\n");
        for (;;)
            ;
    }

    char *buffer = malloc(kernel.info.PhysicalSize);
    if (buffer == NULL)
    {
        printf("ERROR: Failed to allocate memory for kernel data buffer\n");
        for (;;)
            ;
    }

    sfs_read(&kernel, &*buffer);
    if (EFI_ERROR(kernel.status))
    {
        printf("ERROR: Failed to read kernel path\n");
        for (;;)
            ;
        ;
    }

    elf_exec_handle *data = load_elf(buffer);
    if (data == NULL)
    {
        printf("ERROR: Failed to load kernel (tried loading as ELF)\n");
        sfs_close(&kernel);
        free(buffer);
        free(path_wide);
        for (;;)
            ;
    }

    sfs_close(&kernel);
    free(buffer);
    free(path_wide);

    // TODO: Setup the env for the kernel and pass shit based on protocol
    systemTable->BootServices->ExitBootServices(imageHandle, 0);
    void (*entry)(void) = (void (*)(void))(uintptr_t)data->entry_point;
    entry();
}