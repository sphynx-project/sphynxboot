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


int mbtowc(wchar_t *pwc, const char *s, size_t n)
{
    if (s == NULL || n == 0)
    {
        return 0;
    }

    unsigned char ch = (unsigned char)s[0];

    if (ch < 0x80)
    {
        // 1-byte sequence: 0xxxxxxx
        if (pwc != NULL)
        {
            *pwc = (wchar_t)ch;
        }
        return 1;
    }
    else if (ch < 0xC0)
    {
        // Invalid byte in UTF-8
        return -1;
    }
    else if (ch < 0xE0)
    {
        // 2-byte sequence: 110xxxxx 10xxxxxx
        if (n < 2)
        {
            return -1;
        }
        if ((s[1] & 0xC0) != 0x80)
        {
            return -1;
        }
        if (pwc != NULL)
        {
            *pwc = ((ch & 0x1F) << 6) | (s[1] & 0x3F);
        }
        return 2;
    }
    else if (ch < 0xF0)
    {
        // 3-byte sequence: 1110xxxx 10xxxxxx 10xxxxxx
        if (n < 3)
        {
            return -1;
        }
        if ((s[1] & 0xC0) != 0x80 || (s[2] & 0xC0) != 0x80)
        {
            return -1;
        }
        if (pwc != NULL)
        {
            *pwc = ((ch & 0x0F) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F);
        }
        return 3;
    }
    else if (ch < 0xF8)
    {
        // 4-byte sequence: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        if (n < 4)
        {
            return -1;
        }
        if ((s[1] & 0xC0) != 0x80 || (s[2] & 0xC0) != 0x80 || (s[3] & 0xC0) != 0x80)
        {
            return -1;
        }
        if (pwc != NULL)
        {
            *pwc = ((ch & 0x07) << 18) | ((s[1] & 0x3F) << 12) | ((s[2] & 0x3F) << 6) | (s[3] & 0x3F);
        }
        return 4;
    }
    else
    {
        // Invalid byte in UTF-8
        return -1;
    }
}

size_t wchar_strlen(const CHAR16 *s)
{
    const CHAR16 *p = s;
    while (*p != L'\0')
    {
        p++;
    }
    return (size_t)(p - s);
}