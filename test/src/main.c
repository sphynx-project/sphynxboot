#include <stdint.h>
#include <sphynxboot.h>

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

void putpixel(framebuffer_t *fb, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
    if (x >= fb->width || y >= fb->height) {
        return;
    }

    uint32_t *pixel_addr = (uint32_t *)(fb->address + y * fb->pitch + x * (fb->bpp / 8));

    uint32_t color = 0xFF000000 | (r << 16) | (g << 8) | b;

    *pixel_addr = color;
}

void _start(boot_t *data) {
    if(data->framebuffer->address == 0) {
        puts("Failed to get framebuffer!\n");
    }

    for(int x = 0; x < 100; x++) {
        for(int y = 0; y < 100; y++) {
            putpixel(data->framebuffer, x, y, 255, 255, 255);
        }
    }
    
    puts("Hello, World!\n");

    hlt();
}