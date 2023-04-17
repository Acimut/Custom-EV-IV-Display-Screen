
# FIRE RED      = BPRE
# ROJO FUEGO    = BPRS
# EMERALD       = BPEE
# ESMERALDA     = BPES
# CFRU      	= CFRU
ROM_CODE ?= BPRE

# [ESP] ------------------------------------------------------------
# Cambiar ff0000 por un offset alineado con suficiente espacio libre.
# Cambiar 0x08 por 0x09 para usar en espacio expandido.
# ejemplo: para el offset 0x1af0000, poner 0x09af0000
#
# [ENG] ------------------------------------------------------------
# Change ff0000 to an aligned offset with enough free space.
# Change 0x08 to 0x09 to use in expanded space.
# example: for offset 0x1af0000, put 0x09af0000
OFFSET ?= 0x08ff0000


# [CFRU]
# en el archivo offsets.ini de tu CFRU, buscar el offset+1 de CompressedMonToMon y ponerlo aquí:
# in your CFRU's offsets.ini file, look for the offset+1 of CompressedMonToMon and put it here:
COMPRESSED_MON_TO_MON_PTR ?= 0x08985AC5

