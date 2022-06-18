.gba
.thumb
.open "BPEE.gba","build/rom_BPEE.gba", 0x08000000
//---------------------------------------------------

//summary screen
//081c0510 l 000000f4 Task_HandleInput
.org 0x081c0510
.area 0xF4, 0xFF
    push    {r4,lr}
    ldr     r3, =Task_HandleInput|1
    bl      bx_r3_Task_HandleInput
    pop     {r4,pc}
bx_r3_Task_HandleInput:
    bx      r3
.pool
.endarea

//081c2afc l 0000013c PutPageWindowTilemaps
.org 0x081c2afc
.area 0x13c, 0xFF
    push    {r4,lr}
    ldr     r3, =PutPageWindowTilemaps|1
    bl      bx_r3_PutPageWindowTilemaps
    pop     {r4,pc}
bx_r3_PutPageWindowTilemaps:
    bx      r3
.pool
.endarea

//081c2c38 l 000000f4 ClearPageWindowTilemaps
.org 0x081c2c38
.area 0x0f4, 0xFF
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
