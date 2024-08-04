/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#ifndef SPHYNXBOOT_H
#define SPHYNXBOOT_H

#include <stdint.h>

// Memory region types
#define MEM_TYPE_FREE      1
#define MEM_TYPE_RESERVED  2
#define MEM_TYPE_ACPI_RECLAIMABLE 3
#define MEM_TYPE_ACPI_NVS  4
#define MEM_TYPE_BADRAM    5

typedef struct memory_region {
    uint64_t base_address;
    uint64_t length;
    uint32_t type;
} memory_region_t;

typedef struct memory_map {
    uint32_t region_count;
    memory_region_t regions[];
} memory_map_t;

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

typedef struct file {
    void *address;
    uint64_t size;
} file_t;

typedef struct boot {
    framebuffer_t *framebuffer;
    info_t *info;
    file_t *ramfs;
    memory_map_t *memory_map;
} boot_t;

#endif // SPHYNXBOOT_H
