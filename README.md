# Custom-EV-IV-Display-Screen
 Plantilla personalizable para mostar información de EV, IV y BS de nuestro equipo pokémon en una nueva pantalla. Pokémon Fire Red.
 
Los archivos dentro de la carpeta src\include\ fueron tomado de pokefirered.
 
Características:
-
+ Puedes cambiar el fondo reemplazando el que viene por defecto, la inyección la insertará automáticamente.
+ Se puede cambiar las coordenadas del sprite del pokémon, así como sus textos.
+ El color de la estadística de IVs cambia de acuerdo a la naturaleza.
+ Muestra estadísticas base de acuerdo a la especie.
+ Censura la estadística de los huevos, en cambio, dice aprox. cúantos pasos te falta para la eclosión.
+ Bien bomnito y con somniditos xd
+ Huele a limón.
+ base: pokémon fire red.


***Notas:***

- DevkitARM y ARMIPS son necesarios (versiones más recientes).

- Para compilar es necesario tener preproc.exe y gbagfx.exe dentro alguna ruta de la variable PATH

- Abrir el archivo Makefile, buscar y cambiar ff0000 de la siguiente línea por un offset alineado con suficiente espacio libre (más de 0x2000 bytes):
        `export INSERT_INTO := 0x08ff0000`

- Usar en un script `callasm` seguido por el offset+1 donde insertaron el código.

- Compilan ejecutando make con su terminal, y una rom llamada "rom.gba" con la inyección aparecerá en una carpeta llamada build.

- Archivos dentro de la carpeta include fueron tomados de pokefirered.

- El archivo incbin.h es tomado del siguiente repositorio: https://github.com/graphitemaster/incbin
