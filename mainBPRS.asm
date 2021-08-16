.gba
.thumb
.open "BPRS0.gba","build/romEsp.gba", 0x08000000
//---------------------------------------------------
.align 4
.org insertinto
.importobj "build/linked.o"
.close