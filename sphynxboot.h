/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#ifndef SPHYNXBOOT_H
#define SPHYNXBOOT_H

// TODO: Dont depend on compiler for int types.
#include <stdint.h>

typedef struct framebuffer
{
    uint64_t address;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint16_t bpp;
    uint8_t  red_mask_size;
    uint8_t  red_mask_shift;
    uint8_t  green_mask_size;
    uint8_t  green_mask_shift;
    uint8_t  blue_mask_size;
    uint8_t  blue_mask_shift;
} framebuffer_t;

typedef struct info {
    char name[16];
} info_t;

typedef struct boot {
    framebuffer_t *framebuffer;
    info_t *info;
} boot_t;

#endif // SPHYNXBOOT_H