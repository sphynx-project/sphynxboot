/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <utils/wchar.h>

size_t utf8_char_to_wchar(const char *utf8_str, CHAR16 *wchar_str) {
    const unsigned char *ptr = (const unsigned char *)utf8_str;
    size_t count = 0;

    while (*ptr) {
        if (*ptr < 0x80) {
            wchar_str[count++] = *ptr;
            ptr++;
        } else if ((*ptr & 0xE0) == 0xC0) {
            wchar_str[count++] = ((*ptr & 0x1F) << 6) | (*(ptr + 1) & 0x3F);
            ptr += 2;
        } else if ((*ptr & 0xF0) == 0xE0) {
            wchar_str[count++] = ((*ptr & 0x0F) << 12) | ((*(ptr + 1) & 0x3F) << 6) | (*(ptr + 2) & 0x3F);
            ptr += 3;
        } else if ((*ptr & 0xF8) == 0xF0) {
            wchar_str[count++] = ((*ptr & 0x07) << 18) | ((*(ptr + 1) & 0x3F) << 12) | ((*(ptr + 2) & 0x3F) << 6) | (*(ptr + 3) & 0x3F);
            ptr += 4;
        } else {
            return -1;
        }
    }

    wchar_str[count] = L'\0';
    return count;
}