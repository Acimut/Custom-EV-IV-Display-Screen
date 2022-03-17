.gba
.thumb
.open "BPRE.gba","build/Pokémon Fire Red 2.gba", 0x08000000
//---------------------------------------------------
.align 4
.org insertinto
.importobj "build/linked.o"
.close
