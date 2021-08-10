OUTPUT_ARCH(arm)
MEMORY {
        rom     : ORIGIN = 0x08000000, LENGTH = 32M
}

SECTIONS {
        .text : {

                FILL (0xABCD)

                __text_start = . ;
                *(.init)
                *(.text)
                *(.text.*)
                *(.ctors)
                *(.dtors)
                *(.data)
                *(.rodata)
                *(.fini)
                *(COMMON)
                __text_end  = . ;

                __bss_start__ = . ;
                *(.bss .bss.*)
                __bss_end__ = . ;
        _end = __bss_end__ ;
        __end__ = __bss_end__ ;
        } >rom = 0xff
}
