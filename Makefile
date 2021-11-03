#-------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error Please set DEVKITARM in your environment. export DEVKITARM=/path/to/devkitARM)
endif

include $(DEVKITARM)/base_tools

#-------------------------------------------------------------------------------

include config.mk

export BUILD := build
export SRC := src
export BINARY := $(BUILD)/linked.o
export RESOURCES := graphics

export ARMIPS ?= armips.exe
export GFX ?= gbagfx.exe
export PREPROC ?= preproc.exe
export LD := $(PREFIX)ld

export ASFLAGS := -mthumb
	
export INCLUDES := -I $(SRC) -I . -I include -D$(ROM_CODE)
export WARNINGFLAGS :=	-Wall -Wno-discarded-array-qualifiers \
	-Wno-int-conversion
export CFLAGS := -g -O2 $(WARNINGFLAGS) -mthumb -std=gnu11 $(INCLUDES) -mcpu=arm7tdmi \
	-march=armv4t -mno-thumb-interwork -fno-inline -fno-builtin -mlong-calls -DROM_$(ROM_CODE) \
	-fdiagnostics-color
export DEPFLAGS = -MT $@ -MMD -MP -MF "$(@:%.o=%.d)"
export LDFLAGS := -T linker.ld -r $(ROM_CODE).ld 

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
C_MAIN := $(call rwildcard,$(SRC),call_eviv.c)
# HEADERS := $(call rwildcard,$(SRC),*.h)
C_SRC := $(call rwildcard,$(SRC),*.c)
S_SRC := $(call rwildcard,$(SRC),*.s)

# Binaries
C_MAIN := $(C_MAIN:%.c=$(BUILD)/%.o)
C_OBJ := $(C_SRC:%.c=$(BUILD)/%.o)
S_OBJ := $(S_SRC:%.s=$(BUILD)/%.o)

ALL_OBJ := $(C_MAIN) $(C_OBJ) $(S_OBJ)

#-------------------------------------------------------------------------------

.PHONY: all rom clean graphics

all: clean graphics rom

rom: main$(ROM_CODE).asm $(BINARY)
	@echo "\nCreating ROM"
	$(ARMIPS) main$(ROM_CODE).asm -definelabel insertinto $(OFFSET) -sym offsets.txt

clean:
	rm -rf $(BINARY)
	rm -rf $(BUILD)/$(SRC)
	rm -rf $(GBA4BPP_LZ)
	rm -rf $(GBAPAL)
	rm -rf $(GBA_BIN_LZ)

$(BINARY): $(ALL_OBJ)
	@echo "\nLinking ELF binary $@"
	@$(LD) $(LDFLAGS) -o $@ $^
	
$(BUILD)/%.o: %.c
	@echo "\nCompiling $<"
	@mkdir -p $(@D)
	@$(CC) $(DEPFLAGS) $(CFLAGS) -E -c $< -o $(BUILD)/$*.i
	@$(PREPROC) $(BUILD)/$*.i charmap.txt | $(CC) $(CFLAGS) -x c -o $@ -c -

$(BUILD)/%.o: %.s
	@echo -e "Assembling $<"
	@mkdir -p $(@D)
	$(PREPROC) "$<" charmap.txt | @$(AS) $(ASFLAGS) -o $@

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

firered:              	; @$(MAKE) ROM_CODE=BPRE
rojofuego:           	; @$(MAKE) ROM_CODE=BPRS
emerald:              	; @$(MAKE) ROM_CODE=BPEE
