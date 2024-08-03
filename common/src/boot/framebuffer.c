/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <boot/framebuffer.h>
#include <lib/print.h>
#include <common.h>
#include <stddef.h>

framebuffer_t load_framebuffer() {
    framebuffer_t fb;
     EFI_STATUS status;

    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    EFI_UINTN SizeOfInfo, numModes, nativeMode;

    status = systemTable->BootServices->LocateProtocol(&gopGuid, NULL, (void **)&gop);
    if (EFI_ERROR(status))
    {
        printf(" - Error: Failed to locate GOP protocol\n");
        return (framebuffer_t){0};
    }

    status = gop->QueryMode(gop, gop->Mode == NULL ? 0 : gop->Mode->Mode, &SizeOfInfo, &info);
    if (status == (EFI_UINTN)EFI_NOT_STARTED)
    {
        status = gop->SetMode(gop, 0);
    }

    if (EFI_ERROR(status))
    {
        printf(" - Error: Unable to get native mode\n");
        return (framebuffer_t){0};
    }
    else
    {

        nativeMode = gop->Mode->Mode;
        numModes = gop->Mode->MaxMode;
    }

    status = gop->QueryMode(gop, nativeMode, &SizeOfInfo, &info);
    if (EFI_ERROR(status))
    {
        printf(" - Error: Unable to get native mode info\n");
        return (framebuffer_t){0};
    }

    status = gop->SetMode(gop, nativeMode);
    if (EFI_ERROR(status))
    {
        printf(" - Error: Unable to set mode\n");
        return (framebuffer_t){0};
    }

    switch (gop->Mode->Info->PixelFormat)
    {
    case PixelBlueGreenRedReserved8BitPerColor:
        fb.bpp = 32;
        fb.red_mask_size = 8;
        fb.red_mask_shift = 16;
        fb.green_mask_size = 8;
        fb.green_mask_shift = 8;
        fb.blue_mask_size = 8;
        fb.blue_mask_shift = 0;
        break;
    case PixelRedGreenBlueReserved8BitPerColor:
        fb.bpp = 32;
        fb.red_mask_size = 8;
        fb.red_mask_shift = 0;
        fb.green_mask_size = 8;
        fb.green_mask_shift = 8;
        fb.blue_mask_size = 8;
        fb.blue_mask_shift = 16;
        break;
    default:
        break;
    }

    fb.address = gop->Mode->FrameBufferBase;
    fb.width = gop->Mode->Info->HorizontalResolution;
    fb.height = gop->Mode->Info->VerticalResolution;
    fb.pitch = gop->Mode->Info->PixelsPerScanLine * (fb.bpp / 8);
    return fb;
}

void putpixel(framebuffer_t *fb, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
    if (x >= fb->width || y >= fb->height) {
        return;
    }

    uint32_t *pixel_addr = (uint32_t *)(fb->address + y * fb->pitch + x * (fb->bpp / 8));

    uint32_t color = 0xFF000000 | (r << 16) | (g << 8) | b;

    *pixel_addr = color;
}