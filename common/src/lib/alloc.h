/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#ifndef ALLOC_H
#define ALLOC_H

#include <common.h>
#include <stddef.h>

void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t new_size);

#endif // ALLOC_H