#include <stdint.h>
#include <sphynxboot.h>

#include "seif.h"

void hlt()
{
    __asm__ volatile("hlt");
    for (;;)
        ;
}

void outb(uint16_t port, uint8_t value) {
    __asm__ volatile("outb %1, %0" : : "dN"(port), "a"(value));
}

void puts(const char* str) {
    while(*str)
        outb(0xE9, *str++);
}

void putptr(uint8_t *data, uint64_t size) {
    for(uint64_t i = 0; i < size; i++)
        outb(0xE9, data[i]);
}

void putpixel(framebuffer_t *fb, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
    if (x >= fb->width || y >= fb->height) {
        return;
    }

    uint32_t *pixel_addr = (uint32_t *)(fb->address + y * fb->pitch + x * (fb->bpp / 8));

    uint32_t color = 0xFF000000 | (r << 16) | (g << 8) | b;

    *pixel_addr = color;
}

void draw_image(framebuffer_t *fb, uint8_t *image) {
    SEIF_Header *header = (SEIF_Header *)image;
    if(header->magic[0] != 'S' || header->magic[1] != 'E' || header->magic[2] != 'I' || header->magic[3] != 'F') {
        puts("ERROR: Failed to draw image: Invalid image magic!\n");
        return;
    }

    puts("Drawing SEIF image.\n");

    uint8_t encoding_size = 0;
    if(header->encoding == SEIF_ENCODING_RGB) {
        encoding_size = 3;
    } else if(header->encoding == SEIF_ENCODING_RGBA || header->encoding == SEIF_ENCODING_ARGB) {
        encoding_size = 4;
    }

    SEIF_ChunkHeader *chunk_header = (SEIF_ChunkHeader *)(image + sizeof(SEIF_Header));
    if(header->chunk_count != 1 || 
       chunk_header->width != header->meta.width || 
       chunk_header->height != header->meta.height) {
        puts("ERROR: Unsupported image format or multiple chunks!\n");
        return;
    }

    uint8_t *data = (uint8_t *)(image + sizeof(SEIF_Header) + sizeof(SEIF_ChunkHeader));

    for(int y = 0; y < header->meta.height; y++) {
        for(int x = 0; x < header->meta.width; x++) {
            int index = (y * header->meta.width + x) * encoding_size;

            uint8_t r = 0, g = 0, b = 0, a = 255;

            if(header->encoding == SEIF_ENCODING_RGB) {
                r = data[index];
                g = data[index + 1];
                b = data[index + 2];
            } else if(header->encoding == SEIF_ENCODING_RGBA) {
                r = data[index];
                g = data[index + 1];
                b = data[index + 2];
                a = data[index + 3];
            } else if(header->encoding == SEIF_ENCODING_ARGB) {
                a = data[index];
                r = data[index + 1];
                g = data[index + 2];
                b = data[index + 3];
            }

            (void)a;
            putpixel(fb, x, y, r, g, b);
        }
    }
}

void _start(boot_t *data) {
    if(data->framebuffer->address == 0) {
        puts("Failed to get framebuffer!\n");
        hlt();
    }

    uint8_t *image = (uint8_t*)data->ramfs->address;

    draw_image(data->framebuffer, image);

    hlt();
}
