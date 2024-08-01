/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <fs/sfs.h>
#include <lib/string.h>

EFI_GUID gEfiLoadedImageProtocolGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
EFI_GUID gEfiSimpleFileSystemProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
EFI_GUID gEfiFileInfoGuid = EFI_FILE_INFO_ID;

EFI_STATUS get_rootdir(EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *rootfs, EFI_FILE_PROTOCOL **rootdir)
{
    return rootfs->OpenVolume(rootfs, rootdir);
}

EFI_STATUS get_rootfs(EFI_HANDLE loader, EFI_SYSTEM_TABLE *system, EFI_HANDLE device, EFI_SIMPLE_FILE_SYSTEM_PROTOCOL **rootfs)
{
    EFI_STATUS status;
    EFI_LOADED_IMAGE_PROTOCOL *loadedImage;

    status = system->BootServices->HandleProtocol(loader, &gEfiLoadedImageProtocolGuid, (void **)&loadedImage);
    if (EFI_ERROR(status))
        return status;

    status = system->BootServices->HandleProtocol(loadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void **)rootfs);
    return status;
}

EFI_STATUS get_loader_image(EFI_HANDLE loader, EFI_SYSTEM_TABLE *system, EFI_LOADED_IMAGE_PROTOCOL **image)
{
    return system->BootServices->HandleProtocol(loader, &gEfiLoadedImageProtocolGuid, (void **)image);
}

SimpleFile sfs_open(CHAR16 *path)
{
    SimpleFile file;
    EFI_STATUS status;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *rootfs;
    EFI_FILE_PROTOCOL *rootdir;

    status = get_rootfs(*imageHandle, systemTable, NULL, &rootfs);
    if (EFI_ERROR(status))
    {
        file.status = status;
        return file;
    }

    status = get_rootdir(rootfs, &rootdir);
    if (EFI_ERROR(status))
    {
        file.status = status;
        return file;
    }

    status = rootdir->Open(rootdir, &file.efiFile, path, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(status))
    {
        file.status = status;
        return file;
    }

    EFI_UINTN infoSize = sizeof(file.info);
    status = file.efiFile->GetInfo(file.efiFile, &gEfiFileInfoGuid, &infoSize, &file.info);
    file.status = status;

    return file;
}

EFI_FILE_INFO sfs_get_info(SimpleFile file)
{
    return file.info;
}

void sfs_read(SimpleFile *file, void *buffer)
{
    EFI_UINTN bufferSize = (EFI_UINTN)file->info.FileSize;
    file->efiFile->Read(file->efiFile, &bufferSize, buffer);
}

void sfs_close(SimpleFile *file)
{
    file->efiFile->Close(file->efiFile);
}
