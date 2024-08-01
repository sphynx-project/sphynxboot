/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#ifndef STRING_H
#define STRING_H

#include <stddef.h>

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *dest, int val, size_t len);
void memzero(void *ptr, size_t size);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
size_t strlen(const char *str);
char *strchr(const char *str, int c);
int strcmp(const char *str1, const char *str2);
char *strstr(const char *haystack, const char *needle);
int strncmp(const char *str1, const char *str2, size_t n);
size_t strspn(const char *str, const char *accept);
size_t strcspn(const char *str, const char *delim);
char *strtok(char *str, const char *delim);

#endif // STRING_H