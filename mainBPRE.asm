.gba
.thumb
.open "BPRE.gba","build/rom_BPRE.gba", 0x08000000
//---------------------------------------------------

//08419c7b g 00000007 gText_PokeSum_Controls_Page
//7b 9c 41 08 encontrado en 0x137DA0
//agrega texto en una ventana.
.org 0x08137DA0
    .word gText_PokeSum_Controls_Page_EvIv


//08134840 l 0000036c Task_InputHandler_Info
/*
.org 0x08134840
Task_InputHandler_Info_hook:
.area 0x36C, 0xFF
        push {r4,lr}
        ldr r3, =New_Task_InputHandler_Info|1
        bl bx_r3_Task_InputHandler_Info
        pop {r4,pc}
bx_r3_Task_InputHandler_Info:
        bx r3
.pool
.endarea*/

.org 0x08134A52
    ldr R4, = hoook_Task_InputHandler_Info|1
    bl bx_r4
    //sb 0x08134b38      //PSS_STATE3270_ATEXIT_FADEOUT
    b 0x08134B9A    //end func
bx_r4:
    bx r4
.pool

.align 4
.org insertinto
.importobj "build/linked.o"
.close
