OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

SECTIONS
{
    . = 2M;

    .text : {
        *(.text)
        *(.text*)
        *(.gnu.linkonce.t.*)
    }

    .rodata : {
        *(.rodata)
        *(.rodata*)
    }

    .data : {
        *(.data)
        *(.data*)
    }

    .bss : {
        *(.bss)
        *(COMMON)
    }

    . = ALIGN(0x1000);

    .stack : {
        . = ALIGN(0x1000);
        *(.stack)
        . = . + 0x10000;
    }

    _end = .;

    PROVIDE(_start = .);
    PROVIDE(_end = .);
}
