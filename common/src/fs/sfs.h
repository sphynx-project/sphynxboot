/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#ifndef SFS_H
#define SFS_H

#include <common.h>
#include <stddef.h>
#include <stdint.h>

#define EFI_FILE_INFO_ID                                                               \
    {                                                                                  \
        0x09576e92, 0x6d3f, 0x11d2, { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } \
    }
    
typedef struct
{
    EFI_STATUS status;
    EFI_FILE_PROTOCOL *efiFile;
    EFI_FILE_INFO info;
} SimpleFile;

EFI_STATUS get_rootdir(EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *rootfs, EFI_FILE_PROTOCOL **rootdir);
EFI_STATUS get_rootfs(EFI_HANDLE loader, EFI_SYSTEM_TABLE *system, EFI_HANDLE device, EFI_SIMPLE_FILE_SYSTEM_PROTOCOL **rootfs);
EFI_STATUS get_loader_image(EFI_HANDLE loader, EFI_SYSTEM_TABLE *system, EFI_LOADED_IMAGE_PROTOCOL **image);

SimpleFile sfs_open(CHAR16 *path);
EFI_FILE_INFO sfs_get_info(SimpleFile file);
void sfs_read(SimpleFile *file, void *buffer);
void sfs_close(SimpleFile *file);

#endif // SFS_H