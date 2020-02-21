
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)


MEMORY
{
  rom (rx)  : ORIGIN = 0x00400000, LENGTH = 0x00200000
  ram (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00028000
}

SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        KEEP(*(.vectors))
	    *(.text )

    } > rom

    .data :
    {
        . = ALIGN(4);
        *(.data);

    } > ram
 
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        *(.bss)

    } > ram
}

