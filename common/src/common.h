/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#ifndef PUB_H
#define PUB_H

#define _X64
#include <efi.h>

// Temporary fix.
typedef EFI_INTN INTN;
#include <efilib.h>

extern EFI_HANDLE *imageHandle;
extern EFI_SYSTEM_TABLE *systemTable;

extern EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *stdout;
extern EFI_SIMPLE_TEXT_INPUT_PROTOCOL *stdin;
extern EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *stderr;

#endif // PUB_H
