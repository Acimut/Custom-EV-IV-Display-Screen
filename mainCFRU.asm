.gba
.thumb
.open "CFRU.gba","build/rom_CFRU.gba", 0x08000000
//---------------------------------------------------

// hook PokeSum_PrintPageHeaderText case PSS_PAGE_SKILLS:

//0x08137D8C:   ldr R0, PokeSum_PrintPageHeaderText_new
.org 0x08137d9c
    .word PokeSum_PrintPageHeaderText_new|1

.org 0x08137D8E
    bl      bx_r0_pokesum   //jump to PokeSum_PrintPageHeaderText_new
    b       0x08137e16      //End of function PokeSum_PrintPageHeaderText
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
