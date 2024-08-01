#include <stdint.h>

void hlt()
{
    __asm__ volatile("hlt");
    for (;;)
        ;
}

void _start() {
    uint8_t value = 'A';
    uint16_t port = 0xE9;
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
    hlt();
}