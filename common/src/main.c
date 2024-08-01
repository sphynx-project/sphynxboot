/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <common.h>
#include <fs/sfs.h>
#include <lib/print.h>
#include <lib/alloc.h>
#include <boot/kernel.h>

EFI_HANDLE *imageHandle;
EFI_SYSTEM_TABLE *systemTable;

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *stdout;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL *stdin;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *stderr;

void enter_firmware_settings()
{
    systemTable->BootServices->Exit(imageHandle, 0, 0, NULL);
}

EFI_STATUS
sphynxboot_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    imageHandle = &ImageHandle;
    systemTable = &*SystemTable;
    stdout = systemTable->ConOut;
    stdin = systemTable->ConIn;
    stderr = systemTable->StdErr;

    stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
    stdout->SetCursorPosition(stdout, 0, 0);
    stdout->ClearScreen(stdout);

    load_kernel();

    for (;;)
        ;
}