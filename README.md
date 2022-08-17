# Custom-EV-IV-Display-Screen
 Plantilla personalizable para mostar información de EV, IV y BS de nuestro equipo pokémon en una nueva pantalla. Pokémon Fire Red.

## Versión: Summary Screen

Los archivos dentro de la carpeta src\include\ fueron tomado de pokefirered.

Características:
-
+ Compatible con Pokémon Fire Red, Rojo Fuego, Emerald y Esmeralda.
+ Puedes cambiar el fondo reemplazando el que viene por defecto, la inyección la insertará automáticamente.
+ Se puede cambiar las coordenadas del sprite del pokémon, así como sus textos.
+ El color de la estadística de IVs cambia de acuerdo a la naturaleza.
+ Muestra estadísticas base de acuerdo a la especie.
+ Censura la estadística de los huevos, en cambio, dice cúantos pasos falta para la eclosión.
+ Muestra el porcentaje de felicidad del pokémon.
+ Ahora muestra el poder y tipo de poder oculto.
+ Se puede compilar los textos en Español o en inglés sólo cambiando una variable en el archivo de configuración.
+ Bonito y con sonidos.
+ Huele a limón.


***Notas:***

- DevkitARM (r54 o superior) y ARMIPS (v0.11.0) son necesarios.

- Para compilar es necesario tener preproc.exe y gbagfx.exe dentro alguna ruta de la variable PATH, o copiarlos en la carpeta raíz del proyecto.

- Para configuraciones básicas, modificar el archivo config.mk desde un editor de texto.

- Abrir el archivo config.mk, buscar y cambiar ff0000 de la siguiente línea por un offset alineado con suficiente espacio libre (más de 0x2000 bytes):
        `INSERT_INTO ?= 0x08ff0000`
- En el archivo config.mk, buscar la siguiente línea
        `ROM_CODE ?= BPRE`

      + dejar BPRE para compilar usando Fire Red
      + cambiar a BPRS para compilar usando Rojo Fuego en español
      + cambiar a BPEE para compilar usando Emerald

- Luego de pegar una copia de tu rom en la carpeta raíz del proyecto, cambian el nombre de la rom al código de la rom:

      FIRE RED:         BPRE.gba
      ROJO FUEGO:       BPRS.gba
      EMERALD:          BPEE.gba
      ESMERALDA:        BPES.gba


- Compilan ejecutando make con su terminal, y una rom con la inyección aparecerá en una carpeta llamada build.

- Ahora no es necesario usar un script, en esta versión la pantalla de ev-iv será llamada desde la summary screen.

- Archivos dentro de la carpeta include fueron tomados de pokefirered.


***Vídeos:***

https://user-images.githubusercontent.com/79547805/185257288-5ea5b0d2-9ef9-4ad0-819b-3ffd7ff50a42.mp4


https://user-images.githubusercontent.com/79547805/185257722-04085199-f72b-49b4-b8e5-2d93db43a7b2.mp4

