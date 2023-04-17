# Custom-EV-IV-Display-Screen
[![en](https://img.shields.io/badge/lang-en-red.svg)](https://github.com/Acimut/Custom-EV-IV-Display-Screen/blob/SummaryScreen/README.en.md)
[![es](https://img.shields.io/badge/lang-es-yellow.svg)](https://github.com/Acimut/Custom-EV-IV-Display-Screen/blob/SummaryScreen/README.md)
-

It shows information of EV(effort values), IV(individual values) and BS(base stats) of our pokémon team in a new screen. GBA Pokemon.
***

## Version: Summary Screen
In this version, `EV-IV Display Screen` is called from the `summary screen`, making it possible to use it not only conventionally but also from PC, during battles and some other scenarios.
***

### **Features:**
+ Compatible whit Pokémon Fire Red (BPRE 1.0), Rojo Fuego (BPRS), Emerald (BPEE) and Esmeralda(BPES).
+ [New!] Compatible whit Complete Fire Red Upgrade (CFRU).
+ You can change the background by replacing the default one, the injection will insert it automatically.
+ You can change the coordinates of the pokemon sprite, as well as its texts.
+ IVs stat text changes color according to nature.
+ Shows base stat according to the species.
+ Censors egg data; instead, it shows how many steps it takes to hatch.
+ Shows the pokemon's happiness percentage.
+ Now shows power and hidden power type.
+ You can compile the texts in Spanish or English by changing a definition in the file `src/main_eviv.c`
+ Nice and with sounds.
+ Looks good.
***

### **build requirements:**
+ Recommended to compile with WSL, Cygwin or Msys2.
+ DevkitARM (r54 or higher) and [ARMIPS (v0.11.0)](https://github.com/Kingcom/armips/releases/download/v0.11.0/armips-v0.11.0-windows-x86.7z) are required.
+ It is necessary to have `preproc.exe` and `gbagfx.exe` inside some path of the PATH variable. [x86](https://www.mediafire.com/file/v1w09jxv9e0f5df/gba-tools_x86.rar/file) [x64](https://www.mediafire.com/file/sdnyh09nbggye1r/gba-tools.rar/file)
***

### **build instructions:**
+ ***Change the insertion offset:*** 
   + Open the config.mk file and find the following line: 

      `OFFSET ?= 0x08ff0000`

      + Change ff0000 to an offset aligned with enough free space (more than 0x2000 bytes).
      + Change `0x08` to `0x09` to use in expanded space.

      example: for offset 0x1af0000, put 0x09af0000


+ ***Change ROM version:***
   + In the config.mk file, find the following line

      `ROM_CODE ?= BPRE`

      + leave BPRE to compile using Fire Red
      + switch to BPRS to compile using Rojo Fuego
      + switch to BPEE to compile using Emerald
      + switch to BPEE to compile using Esmeralda
      + switch to CFRU to compile using CFRU


+ ***If CFRU is used:***
   + In the config.mk file, find the following line

      `COMPRESSED_MON_TO_MON_PTR ?= 0x08985AC5`

   + go to your CFRU's offsets.ini file, look for the CompressedMonToMon +1 offset, and put it in place of `08985AC5` in the config.mk file

+ ***Paste*** a copy of your rom in the root folder of the project, change the name of the rom to the corresponding code:

      FIRE RED:         BPRE.gba
      ROJO FUEGO:       BPRS.gba
      EMERALD:          BPEE.gba
      ESMERALDA:        BPES.gba
      CFRU:             CFRU.gba

+ ***Compile*** by running `make` with your terminal, and a rom with the injection will appear in a folder called `build`.
***

### **other configurations:**
+ In a script you can enable/disable the ev-iv display screen using a `FLAG_EV_IV` flag which is defined in `src/main_eviv.c`. This flag is set by default to `FLAG_SYS_POKEMON_GET` for when we receive the first pokemon, but it can be changed for other purposes.

In the `src/main_eviv.c` file are the following preprocessor directive definitions, which we'll use to make some quick changes to the code before compiling:
+ ***EV_IV_TEXT:*** Change to
    + `LANGUAGE_SPANISH` to compile the texts in Spanish (default).
    + `LANGUAGE_ENGLISH` to compile the texts in English.
+ ***HIDDEN_POWER_STATIC:*** Set to `TRUE` if Hidden Power's base power is fixed. Leave `FALSE` if dynamic (default).
+ ***HIDDEN_POWER_BASE_POWER:*** Change this to the base power of Hidden Power if it is set to any number other than 60. No need to change it if `HIDDEN_POWER_STATIC` is `FALSE`.
+ ***SPRITE_JUMP:*** Activates or deactivates the jump of the sprite of the pokémon at the beginning. Switch to
    + `TRUE` to enable (default).
    + `FALSE` to disable.
+ ***SPRITE_JUMP_DIRECTION:*** Defines the direction of the pokemon sprite's jump; only valid if `SPRITE_JUMP` is `TRUE`. Switch to
    + `1` to jump from right to left (default).
    + `0` so that the jump is in the center.
    + `-1` to jump from left to right.
+ ***SPRITE_VIEW_DIRECTION:*** Defines the direction the pokémon sprite faces. Switch to
    + `0` to make it face to the left (default).
    + `1` to make it face to the right.
+ ***PICMON_X:*** x coordinate of the pokémon sprite on screen, measured in 8 pixel tiles.
+ ***PICMON_Y:*** y coordinate of the pokémon sprite on the screen, measured in 8 pixel tiles.

***

### **Notes:**
+ Now it is not necessary to use a script, in this version the ev-iv screen will be called from the pokémon summary screen.
+ Files inside the `include` folder were taken from [**pokefirered**](https://github.com/pret/pokefirered).
***

### **Videos:**

https://user-images.githubusercontent.com/79547805/185257288-5ea5b0d2-9ef9-4ad0-819b-3ffd7ff50a42.mp4


https://user-images.githubusercontent.com/79547805/185257722-04085199-f72b-49b4-b8e5-2d93db43a7b2.mp4

