.gba
.thumb
.open "BPRE0.gba","build/rom.gba", 0x08000000
//---------------------------------------------------
.align 4
.org insertinto
.importobj "build/linked.o"
.close
