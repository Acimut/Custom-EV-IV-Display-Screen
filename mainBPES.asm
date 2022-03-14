.gba
.thumb
.open "BPES.gba","build/rom_BPES.gba", 0x08000000
//---------------------------------------------------
.align 4
.org insertinto
.importobj "build/linked.o"
.close
