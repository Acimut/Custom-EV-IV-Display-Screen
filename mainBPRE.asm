.gba
.thumb
.open "BPRE.gba","build/rom_BPRE.gba", 0x08000000
//---------------------------------------------------

//08137d28 l 00000100 PokeSum_PrintPageHeaderText
.org 0x08137D44 //0x08137D8C -> 8C 7D 13 08
    .word   PokeSum_PrintPageHeaderText_hook


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

.align 4
PokeSum_PrintPageHeaderText_hook:
    ldr     r3, = PokeSum_PrintPageHeaderText_new|1
    bl      bx_r3_PokeSum_PrintPageHeaderText
    pop     {r0}
    bx      r0
bx_r3_PokeSum_PrintPageHeaderText:
    bx      r3
.pool

.close
