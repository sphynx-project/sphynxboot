/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <boot/kernel.h>
#include <data/elf.h>
#include <utils/wchar.h>
#include <lib/print.h>
#include <lib/string.h>
#include <lib/alloc.h>
#include <fs/sfs.h>
#include <efi.h>

#define PAGE_SIZE 4096
#define PML4_ENTRIES 512
#define PDP_ENTRIES 512
#define PD_ENTRIES 512
#define PT_ENTRIES 512
#define ADDRESS_MASK 0xFFFFFFFFFFFFF000

typedef struct {
    uint64_t entries[PML4_ENTRIES];
} PageTable;

PageTable pml4 __attribute__((aligned(PAGE_SIZE)));
PageTable pdpt __attribute__((aligned(PAGE_SIZE)));
PageTable pd __attribute__((aligned(PAGE_SIZE)));
PageTable pt __attribute__((aligned(PAGE_SIZE)));

// Helper functions for page table management
void clear_page_table(PageTable *table) {
    memset(table, 0, sizeof(PageTable));
}

void create_pml4_entry(PageTable *pml4_table, void *self_addr) {
    uint64_t index = ((uint64_t)self_addr >> 39) & 0x1FF;
    pml4_table->entries[index] = (uint64_t)pml4_table | 0x03; // Present and writable
}

EFI_STATUS map_page(PageTable *page_map, uint64_t virtual_addr, uint64_t physical_addr, uint64_t num_pages) {
    uint64_t pml4_idx, pdpt_idx, pd_idx, pt_idx;
    uint64_t pml4_entry, pdpt_entry, pd_entry, pt_entry;
    uint64_t page_frame_number = physical_addr >> 12;
    EFI_STATUS status;

    while (num_pages > 0) {
        pml4_idx = (virtual_addr >> 39) & 0x1FF;
        pdpt_idx = (virtual_addr >> 30) & 0x1FF;
        pd_idx = (virtual_addr >> 21) & 0x1FF;
        pt_idx = (virtual_addr >> 12) & 0x1FF;

        if (!(page_map->entries[pml4_idx] & 0x1)) {
            page_map->entries[pml4_idx] = (uint64_t)&pdpt | 0x03;
            clear_page_table(&pdpt);
        }
        pml4_entry = page_map->entries[pml4_idx];

        PageTable *pdpt_table = (PageTable *)(pml4_entry & ADDRESS_MASK);
        if (!(pdpt_table->entries[pdpt_idx] & 0x1)) {
            pdpt_table->entries[pdpt_idx] = (uint64_t)&pd | 0x03;
            clear_page_table(&pd);
        }
        pdpt_entry = pdpt_table->entries[pdpt_idx];

        PageTable *pd_table = (PageTable *)(pdpt_entry & ADDRESS_MASK);
        if (!(pd_table->entries[pd_idx] & 0x1)) {
            pd_table->entries[pd_idx] = (uint64_t)&pt | 0x03;
            clear_page_table(&pt);
        }
        pd_entry = pd_table->entries[pd_idx];

        PageTable *pt_table = (PageTable *)(pd_entry & ADDRESS_MASK);
        pt_table->entries[pt_idx] = (page_frame_number << 12) | 0x03; // Present and writable

        virtual_addr += PAGE_SIZE;
        page_frame_number++;
        num_pages--;
    }

    return EFI_SUCCESS;
}

void setup_higher_half_mapping(void) {
    uint64_t high_base = 0xffffffff80000000;
    uint64_t high_limit = 0xffffffffffffffff;
    uint64_t num_pages = (high_limit - high_base) / PAGE_SIZE;

    for (uint64_t addr = high_base; addr < high_limit; addr += PAGE_SIZE) {
        map_page(&pml4, addr, addr - high_base, num_pages);
    }
}

EFI_STATUS retrieve_memory_map(EFI_MEMORY_DESCRIPTOR **map, EFI_UINTN *map_size, EFI_UINTN *desc_size, EFI_UINT32 *desc_version) {
    EFI_STATUS status;
    EFI_UINTN buffer_size = 0;
    EFI_MEMORY_DESCRIPTOR *buffer = NULL;

    status = systemTable->BootServices->GetMemoryMap(&buffer_size, buffer, NULL, desc_size, desc_version);
    if (status != EFI_BUFFER_TOO_SMALL) {
        return status;
    }

    buffer = malloc(buffer_size);
    if (buffer == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }

    status = systemTable->BootServices->GetMemoryMap(&buffer_size, buffer, NULL, desc_size, desc_version);
    if (EFI_ERROR(status)) {
        free(buffer);
        return status;
    }

    *map = buffer;
    *map_size = buffer_size;
    return EFI_SUCCESS;
}

void load_kernel(char *path) {
    char kernel_path[256];
    char *p;

    strncpy(kernel_path, path, sizeof(kernel_path) - 1);
    kernel_path[sizeof(kernel_path) - 1] = '\0';

    for (p = kernel_path; *p; ++p) {
        if (*p == '/') {
            *p = '\\';
        }
    }

    printf(" - Info: Loading %s\n", path);

    EFI_MEMORY_DESCRIPTOR *map = NULL;
    EFI_UINTN map_size = 0;
    EFI_UINTN desc_size = 0;
    EFI_UINT32 desc_version = 0;

    EFI_STATUS status = retrieve_memory_map(&map, &map_size, &desc_size, &desc_version);
    if (EFI_ERROR(status)) {
        printf(" - Error: Failed to get memory map: %d\n", status);
        for (;;);
    }

    setup_higher_half_mapping();
    printf(" - Info: Mapped higher half\n");

    CHAR16 *path_wide = malloc(strlen(kernel_path) * sizeof(CHAR16) + 2);
    utf8_char_to_wchar(kernel_path, path_wide);

    SimpleFile kernel = sfs_open(path_wide);
    if (EFI_ERROR(kernel.status)) {
        printf(" - Error: Failed to open \"%s\": File not found\n", path);
        for (;;);
    }

    char *buffer = malloc(kernel.info.PhysicalSize);
    if (buffer == NULL) {
        printf(" - Error: Failed to allocate memory for kernel data buffer\n");
        for (;;);
    }

    sfs_read(&kernel, &*buffer);
    if (EFI_ERROR(kernel.status)) {
        printf(" - Error: Failed to read kernel data: %d\n", kernel.status);
        for (;;);
    }

    elf_exec_handle *data = load_elf(buffer);
    if (data == NULL) {
        printf(" - Error: Failed to load kernel: Invalid ELF file\n");
        sfs_close(&kernel);
        free(buffer);
        free(path_wide);
        for (;;);
    }

    sfs_close(&kernel);
    free(buffer);
    free(path_wide);

    printf("%s: 0x%llx - ready", path, data->entry_point);

    systemTable->BootServices->ExitBootServices(imageHandle, 0);
    void (*entry)(void) = (void (*)(void))(uintptr_t)data->entry_point;
    entry();
}
