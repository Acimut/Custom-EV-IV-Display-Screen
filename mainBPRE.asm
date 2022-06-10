.gba
.thumb
.open "BPRE.gba","build/BPRE.gba", 0x08000000
//---------------------------------------------------
.align 4
.org insertinto
.importobj "build/linked.o"
.close
