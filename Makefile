#-------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error Please set DEVKITARM in your environment. export DEVKITARM=/path/to/devkitARM)
endif

include $(DEVKITARM)/base_tools

#-------------------------------------------------------------------------------

export ROM_CODE := BPRE
export INSERT_INTO := 0x08ff0000

export BUILD := build
export SRC := src
export BINARY := $(BUILD)/linked.o
export RESOURCES := $(SRC)/graphics

export ARMIPS ?= armips.exe
export GFX ?= gbagfx.exe
export PREPROC ?= preproc.exe
export LD := $(PREFIX)ld

export ASFLAGS := -mthumb
	
export INCLUDES := -I $(SRC)
export WARNINGFLAGS :=	-Wall -Wno-discarded-array-qualifiers \
	-Wno-int-conversion
export CFLAGS := -O2 -Wextra -mthumb -mno-thumb-interwork -mcpu=arm7tdmi -mtune=arm7tdmi \
	-march=armv4t -mlong-calls -fno-inline -fno-builtin -std=gnu11 -mabi=apcs-gnu -x c -c \
	-MMD $(WARNINGFLAGS) $(INCLUDES) -O -finline 

export LDFLAGS := --relocatable -T linker.ld -T $(ROM_CODE).ld 

#-------------------------------------------------------------------------------
	
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Recursos
IMAGES:= $(call rwildcard,$(RESOURCES),*.png)
TILEMAP:= $(call rwildcard,$(RESOURCES),*.bin)

# tileset comprimido
GBA4BPP := $(IMAGES:%.png=%.4bpp)
GBA4BPP_LZ := $(GBA4BPP:%=%.lz)
# paletas
GBA_PAL := $(IMAGES:%.png=%.pal)
GBAPAL := $(GBA_PAL:%.pal=%.gbapal)
# tilemap comprimido
GBA_BIN_LZ := $(TILEMAP:%=%.lz)

# Sources
# C_MAIN función principal que inicializa el sistema. Se compila primero que nada.
C_MAIN := $(call rwildcard,$(SRC),MainEvIv.c)
HEADERS := $(call rwildcard,$(SRC),*.h)
C_SRC := $(call rwildcard,$(SRC),*.c)
S_SRC := $(call rwildcard,$(SRC),*.s)

# Binaries
C_MAIN := $(C_MAIN:%=$(BUILD)/%.o)
C_OBJ := $(C_SRC:%=$(BUILD)/%.o)
S_OBJ := $(S_SRC:%=$(BUILD)/%.o)

ALL_OBJ := $(C_MAIN) $(C_OBJ) $(S_OBJ)

#-------------------------------------------------------------------------------

.PHONY: all rom clean graphics

all: clean graphics rom

rom: main$(ROM_CODE).asm $(BINARY)
	@echo -e "Creating ROM"
	$(ARMIPS) main$(ROM_CODE).asm -definelabel insertinto $(INSERT_INTO) -sym offsets.txt

clean:
	rm -rf $(BINARY)
	rm -rf $(BUILD)/$(SRC)
	rm -rf $(GBA4BPP_LZ)
	rm -rf $(GBAPAL)
	rm -rf $(GBA_BIN_LZ)

$(BINARY): $(ALL_OBJ)
	@echo -e "Linking ELF binary $@"
	@$(LD) $(LDFLAGS) -o $@ $^
	
$(BUILD)/%.c.o: %.c $(HEADERS)
	@echo -e "Compiling $<"
	@mkdir -p $(@D)
	$(PREPROC) "$<" charmap.txt | $(CC) $(CFLAGS) -MF "$(@:%.o=%.d)" -MT "$@" -o "$@" -

$(BUILD)/%.s.o: %.s $(HEADERS)
	@echo -e "Assembling $<"
	@mkdir -p $(@D)
	@$(AS) $(ASFLAGS) -o $@

%.4bpp: %.png  ; $(GFX) $< $@
%.pal: %.png  ; $(GFX) $< $@
%.gbapal: %.pal ; $(GFX) $< $@ -num_colors 16
%.lz: % ; $(GFX) $< $@

pngto4bpp: $(GBA4BPP)

gba4bpptolz: $(GBA4BPP_LZ)
	rm -rf $(GBA4BPP)

pngtopal: $(GBA_PAL)

paltogbapal: $(GBAPAL)
	rm -rf $(GBA_PAL)

bintolz: $(GBA_BIN_LZ)

graphics: pngto4bpp gba4bpptolz pngtopal paltogbapal bintolz