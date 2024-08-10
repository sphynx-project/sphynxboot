/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#ifndef SPHYNXBOOT_H
#define SPHYNXBOOT_H

// Integer types
typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

// Memory region types
#define MEMMAP_USABLE                 1
#define MEMMAP_RESERVED               2
#define MEMMAP_ACPI_RECLAIMABLE       3
#define MEMMAP_ACPI_NVS               4
#define MEMMAP_BAD_MEMORY             5
#define MEMMAP_BOOTLOADER_RECLAIMABLE 0x1000
#define MEMMAP_EFI_RECLAIMABLE        0x2000

typedef struct memory_region {
    u64 base_address;
    u64 length;
    u32 type;
} memory_region_t;

typedef struct memory_map {
    u32 region_count;
    memory_region_t **regions;
} memory_map_t;

typedef struct framebuffer
{
    u64 address;
    u32 width;
    u32 height;
    u32 pitch;
    u16 bpp;
    u8  red_mask_size;
    u8  red_mask_shift;
    u8  green_mask_size;
    u8  green_mask_shift;
    u8  blue_mask_size;
    u8  blue_mask_shift;
} framebuffer_t;

typedef struct info {
    char name[16];
} info_t;

typedef struct file {
    void *address;
    u64 size;
} file_t;

typedef struct boot {
    framebuffer_t *framebuffer;
    info_t *info;
    file_t *ramfs;
    memory_map_t *memory_map;
} boot_t;

#endif // SPHYNXBOOT_H
