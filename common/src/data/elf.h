/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#ifndef ELF_H
#define ELF_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define EI_NIDENT 16
#define EI_MAG0 0
#define EI_MAG1 1
#define EI_MAG2 2
#define EI_MAG3 3
#define EI_CLASS 4
#define EI_DATA 5
#define EI_VERSION 6

#define ELFMAG0 0x7f
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'

#define ELFCLASS64 2
#define ELFDATA2LSB 1
#define EV_CURRENT 1

#define ET_EXEC 2
#define EM_X86_64 62

#define PT_LOAD 1
#define PT_PHDR 6

typedef unsigned short elf_half;
typedef unsigned int elf_word;
typedef unsigned long long elf_xword;
typedef unsigned long long elf_addr;
typedef unsigned long long elf_off;

typedef struct
{
    unsigned char ident[EI_NIDENT];
    elf_half type;
    elf_half machine;
    elf_word version;
    elf_addr entry;
    elf_off phoff;
    elf_off shoff;
    elf_word flags;
    elf_half ehsize;
    elf_half phentsize;
    elf_half phnum;
    elf_half shentsize;
    elf_half shnum;
    elf_half shstrndx;
} __attribute__((packed)) elf_header;

typedef struct
{
    elf_word type;
    elf_word flags;
    elf_off offset;
    elf_addr vaddr;
    elf_addr paddr;
    elf_xword filesz;
    elf_xword memsz;
    elf_xword align;
} __attribute__((packed)) elf_program_header;

typedef struct {
    void *entry_point;
} elf_exec_handle;

elf_exec_handle *load_elf(const void *data);

#endif // ELF_H
