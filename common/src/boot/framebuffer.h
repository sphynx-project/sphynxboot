/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <sphynxboot.h>

framebuffer_t load_framebuffer();
void putpixel(framebuffer_t *fb, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);

#endif // FRAMEBUFFER_H