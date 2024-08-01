#include <stdint.h>

void hlt()
{
    __asm__ volatile("hlt");
    for (;;)
        ;
}

void putc(char ch) {
    uint16_t port = 0xE9;
    __asm__ volatile ("outb %0, %1" : : "a"(ch), "Nd"(port));
}

void _start() {
    putc('\033');
    putc('c');
    putc('A');
    putc('B');
    putc('C');
    hlt();
}