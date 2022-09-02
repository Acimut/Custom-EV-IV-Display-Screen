.gba
.thumb
.open "BPRE.gba","build/rom_BPRE.gba", 0x08000000
//---------------------------------------------------

//0x08137D8C
.org 0x08137d9c
    .word PokeSum_PrintPageHeaderText_new|1

.org 0x08137D8E
    bl      bx_r0_pokesum
    b       0x08137e16
bx_r0_pokesum:
    bx      r0


//08134840 l 0000036c Task_InputHandler_Info
.org 0x08134840
Task_InputHandler_Info_hook:
.area 0x36C, 0xFF
    push    {r4,lr}
    ldr     r3, =New_Task_InputHandler_Info|1
    bl      bx_r3_Task_InputHandler_Info
    pop     {r4,pc}
bx_r3_Task_InputHandler_Info:
    bx      r3
.pool
.endarea

.align 4
.org insertinto
.importobj "build/linked.o"

.close
