.gba
.thumb
.open "BPEE.gba","build/rom_BPEE.gba", 0x08000000
//---------------------------------------------------
.align 4
.org insertinto
.importobj "build/linked.o"
.close
