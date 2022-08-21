.gba
.thumb
.open "BPES.gba","build/rom_BPES.gba", 0x08000000
//---------------------------------------------------

    //summary screen
//081C0130 l 000000f4 Task_HandleInput
.org 0x081C0130
.area 0xF4//, 0xFF
    push    {r4,lr}
    ldr     r3, =Task_HandleInput|1
    bl      bx_r3_Task_HandleInput
    pop     {r4,pc}
bx_r3_Task_HandleInput:
    bx      r3
.pool
.endarea

/*
b/t 0x081c271C
ok

*/
//081c271C l 0000013c PutPageWindowTilemaps
.org 0x081c271C
.area 0x13c//, 0xFF
    push    {r4,lr}
    ldr     r3, =PutPageWindowTilemaps|1
    bl      bx_r3_PutPageWindowTilemaps
    pop     {r4,pc}
bx_r3_PutPageWindowTilemaps:
    bx      r3
.pool
.endarea

//081c2858 l 000000f4 ClearPageWindowTilemaps
.org 0x081c2858
.area 0x0f4//, 0xFF
    push    {r4,lr}
    ldr     r3, =ClearPageWindowTilemaps|1
    bl      bx_r3_ClearPageWindowTilemaps
    pop     {r4,pc}
bx_r3_ClearPageWindowTilemaps:
    bx      r3
.pool
.endarea



.align 4
.org insertinto
.importobj "build/linked.o"
.close
