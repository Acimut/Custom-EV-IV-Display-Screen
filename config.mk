# Variables


# FIRE RED      = BPRE
# ROJO FUEGO    = BPRS
# EMERALD       = BPEE
# ESMERALDA     = BPES
ROM_CODE ?= BPRE

# [ESP] ------------------------------------------------------------
# Cambiar ff0000 por un offset alineado con suficiente espacio libre.
# Cambiar 0x08 por 0x09 para usar en espacio expandido.
# ejemplo: para el offset 0x1af0000, poner 0x09af0000
# [ENG] ------------------------------------------------------------
# Change ff0000 to an aligned offset with enough free space.
# Change 0x08 to 0x09 to use in expanded space.
# example: for offset 0x1af0000, put 0x09af0000
OFFSET ?= 0x08ff0000


# ------------------------------------------------------------------ 
#                           FIRE RED/ROJOFUEGO     EMERALD/ESMERALDA
# FLAG_SYS_POKEMON_GET      0x828                  0x860
# FLAG_SYS_POKEDEX_GET      0x829                  0x861
# [ESP] ------------------------------------------------------------ 
# Cambiar FLAG_SYS_POKEMON_GET por la flag que quieras usar.
# ejemplo: FLAG_EV_IV ?= 0x200
# [ENG] ------------------------------------------------------------
# Change FLAG_SYS_POKEMON_GET to the flag you want to use.
# example: FLAG_EV_IV ?= 0x200
FLAG_EV_IV ?= FLAG_SYS_POKEMON_GET




