/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <lib/alloc.h>
#include <lib/print.h>

void *malloc(size_t size)
{
    void *ptr;
    if (EFI_ERROR(systemTable->BootServices->AllocatePool(EfiLoaderData, size, &ptr)))
    {
        printf("ERROR: Failed to allocate memory\n");
        return NULL;
    }

    return ptr;
}

void free(void *ptr)
{
    systemTable->BootServices->FreePool(ptr);
}

void CopyMem(void *dest, const void *src, size_t size)
{
    // Copy memory from src to dest
    char *d = dest;
    const char *s = src;
    while (size--)
    {
        *d++ = *s++;
    }
}

void *realloc(void *ptr, size_t new_size)
{
    if (!ptr)
        return malloc(new_size);

    void *new_ptr = malloc(new_size);
    if (!new_ptr)
        return NULL;

    CopyMem(new_ptr, ptr, new_size);
    free(ptr);
    return new_ptr;
}
