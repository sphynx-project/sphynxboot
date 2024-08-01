/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#ifndef WCHAR_H
#define WCHAR_H

#include <common.h>
#include <stdint.h>
#include <stddef.h>

size_t utf8_char_to_wchar(const char *utf8_str, CHAR16 *wchar_str);

#endif // WCHAR_H