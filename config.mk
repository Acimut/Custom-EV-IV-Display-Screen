
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



