/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <data/elf.h>
#include <lib/print.h>
#include <lib/string.h>
#include <lib/alloc.h>

bool _validate_header(elf_header *header)
{
    if (header == NULL)
    {
        printf(" - Error: NULL ELF header passed!\n");
        return false;
    }

    if (header->ident[EI_MAG0] == ELFMAG0 && header->ident[EI_MAG1] == ELFMAG1 && header->ident[EI_MAG2] == ELFMAG2 && header->ident[EI_MAG3] == ELFMAG3)
    {
        if (header->type != ET_EXEC)
        {
            return false;
        }

        if (header->machine != EM_X86_64)
        {
            return false;
        }

        if (header->version != EV_CURRENT)
        {
            return false;
        }

        return true;
    }

    return false;
}

elf_exec_handle *load_elf(const void *data)
{
    elf_exec_handle *handle = malloc(sizeof(elf_exec_handle));
    elf_header *header = (elf_header *)data;
    if (!_validate_header(header))
    {
        printf(" - Error: Invalid ELF header!\n");
        return NULL;
    }

    if (header->phnum <= 0)
    {
        printf(" - Error: No loadable segments found.\n");
        return NULL;
    }

    elf_program_header *program_headers = (elf_program_header *)(data + header->phoff);
    for (uint16_t i = 0; i < header->phnum; i++)
    {
        elf_program_header *program_header = (elf_program_header *)program_headers;
        if (program_header->type == PT_LOAD)
        {
            void *file_segment = (void *)((uint64_t)data + program_header->offset);
            void *mem_segment = (void *)program_header->vaddr;

            memcpy(mem_segment, file_segment, program_header->filesz);

            uint8_t *extra_zeroes = (uint8_t *)mem_segment + program_header->filesz;
            uint64_t extra_zeroes_count = program_header->memsz - program_header->filesz;

            if (extra_zeroes_count > 0)
            {
                memset(extra_zeroes, 0x00, extra_zeroes_count);
            }
        }
        program_headers = (elf_program_header *)((uint8_t*)program_headers + header->phentsize);
    }

    handle->entry_point = (void *)header->entry;

    return handle;
}