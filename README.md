# Custom-EV-IV-Display-Screen
[![en](https://img.shields.io/badge/lang-en-red.svg)](https://github.com/Acimut/Custom-EV-IV-Display-Screen/blob/SummaryScreen/README.en.md)
[![es](https://img.shields.io/badge/lang-es-yellow.svg)](https://github.com/Acimut/Custom-EV-IV-Display-Screen/blob/SummaryScreen/README.md)
-

Muestra información de EV(puntos de esfuerzo), IV(valores individuales) y BS(estadísticas base) de nuestro equipo pokémon en una nueva pantalla. Pokémon de GBA.
***

## Versión: Summary Screen
En esta versión, `EV-IV Display Screen` es llamado desde la `summary screen`, haciendo posible de usar no solo de manera convencional sino también desde la PC, durante las batallas y algunos otros escenarios.
***

### **Características:**
+ Compatible con Pokémon Fire Red (BPRE 1.0), Rojo Fuego (BPRS), Emerald (BPEE) y Esmeralda(BPES).
+ Puedes cambiar el fondo reemplazando el que viene por defecto, la inyección la insertará automáticamente.
+ Se puede cambiar las coordenadas del sprite del pokémon, así como sus textos.
+ El texto de la estadística de IVs cambia de color de acuerdo a la naturaleza.
+ Muestra estadísticas base de acuerdo a la especie.
+ Censura datos de los huevos; en cambio, muestra cúantos pasos falta para la eclosión.
+ Muestra el porcentaje de felicidad del pokémon.
+ Ahora muestra el poder y tipo de poder oculto.
+ Se puede compilar los textos en Español o en Inglés cambiando una definición en el archivo `src/main_eviv.c`
+ Bonito y con sonidos.
+ Huele a limón.
***

### **Requisitos de compilación:**
+ Recomendado compilar con WSL, Cygwin o Msys2.
+ DevkitARM (r54 o superior) y ARMIPS (v0.11.0) son necesarios.
+ Es necesario tener `preproc.exe` y `gbagfx.exe` dentro alguna ruta de la variable PATH.
***

### **Instrucciones de compilación:**
+ ***Cambiar el offset de inserción:*** 
   + Abrir el archivo config.mk y buscar la siguiente línea: 

      `OFFSET ?= 0x08ff0000`

      + Cambiar ff0000 por un offset alineado con suficiente espacio libre (más de 0x2000 bytes).
      + Cambiar `0x08` por `0x09` para usar en espacio expandido.

      ejemplo: para el offset 0x1af0000, poner 0x09af0000


+ ***Cambiar la versión de la ROM:***
   + En el archivo config.mk, buscar la siguiente línea

      `ROM_CODE ?= BPRE`

      + dejar BPRE para compilar usando Fire Red
      + cambiar a BPRS para compilar usando Rojo Fuego
      + cambiar a BPEE para compilar usando Emerald
      + cambiar a BPEE para compilar usando Esmeralda

+ ***Pegar*** una copia de tu rom en la carpeta raíz del proyecto, cambian el nombre de la rom al código correspondiente:

      FIRE RED:         BPRE.gba
      ROJO FUEGO:       BPRS.gba
      EMERALD:          BPEE.gba
      ESMERALDA:        BPES.gba

+ ***Compilar*** ejecutando `make` con su terminal, y una rom con la inyección aparecerá en una carpeta llamada `build`.
***

### **Otras configuraciones:**
+ Es posible activar/desactivar el ev-iv display screen haciendo uso de una flag `FLAG_EV_IV` que se configura desde `src/main_eviv.h`. Esta flag está configurada por defecto con `FLAG_SYS_POKEMON_GET` para cuando recibimos el primer pokémon, pero se puede cambiar para otros propósitos.

En el archivo `src/main_eviv.c` se encuentran las siguientes definiciones de directiva de preprocesador, las cuales usaremos para realizar algunos cambios rápidos al código antes de compilar:
+ ***EV_IV_TEXT:*** Cambiar a 
   + `LANGUAGE_SPANISH` para compilar los textos en español (por defecto).
   + `LANGUAGE_ENGLISH` para compilar los textos en inglés.
+ ***HIDDEN_POWER_STATIC:*** Cambiar a `TRUE` si la potencia base de Poder Oculto es fija. Dejar en `FALSE` si es dinámico (por defecto).
+ ***HIDDEN_POWER_BASE_POWER:*** Cambie esto a la potencia base de Poder Oculto si está fijo en cualquier número que no sea 60. No es necesario modificarlo si `HIDDEN_POWER_STATIC` es `FALSE`.
+ ***SPRITE_JUMP:*** Activa o desactiva el salto del sprite del pokémon al inicio. Cambiar a
   + `TRUE` para activar (por defecto).
   + `FALSE` para desactivar.
+ ***SPRITE_JUMP_DIRECTION:*** Define el sentido del salto del sprite del pokémon; sólo válido si  `SPRITE_JUMP`  es `TRUE`. Cambiar a
   + `1` para que el salto sea de derecha a izquierda (por defecto).
   + `0` para que el salto quede en el centro.
   + `-1` para que el salto sea de izquierda a derecha.
+ ***SPRITE_VIEW_DIRECTION:*** Define la dirección a la que mira el sprite del pokémon. Cambiar a
   + `1` para que mira a la derecha (por defecto).
   + `0` para que mira a la izquierda.
+ ***PICMON_X:*** coordenada x del sprite pokémon en pantalla, se mide en tiles de 8 pixeles.
+ ***PICMON_Y:*** coordenada y del sprite pokémon en pantalla, se mide en tiles de 8 pixeles.

***

### **Notas:**
+ Ahora no es necesario usar un script, en esta versión la pantalla de ev-iv será llamada desde la pantalla de resumen de pokémon (summary screen).
+ Archivos dentro de la carpeta `include` fueron tomados de [**pokefirered**](https://github.com/pret/pokefirered).
***

### **Vídeos:**

https://user-images.githubusercontent.com/79547805/185257288-5ea5b0d2-9ef9-4ad0-819b-3ffd7ff50a42.mp4


https://user-images.githubusercontent.com/79547805/185257722-04085199-f72b-49b4-b8e5-2d93db43a7b2.mp4

