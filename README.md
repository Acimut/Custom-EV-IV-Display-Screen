# Custom-EV-IV-Display-Screen
 Plantilla personalizable para mostar información de EV, IV y BS de nuestro equipo pokémon en una nueva pantalla. Pokémon Fire Red.
 
Los archivos dentro de la carpeta src\include\ fueron tomado de pokefirered.
 
 **Características:**
 *
+ Eliges dónde está ubicado el fondo, modificando el offset el archivo `BPRE.ld`
+ Se puede cambiar la ubicación del sprite del pokémon, así como sus textos.
+ El color de la estadística de IVs cambia de acuerdo a la naturaleza.
+ Muestra estadísticas base de acuerdo a la especie.
+ Censura la estadística de los huevos, en cambio, dice aprox. cúantos pasos te falta para la eclosión.
+ Bien bomnito y con somniditos xd
+ Huele a limón.
+ base: pokémon fire red.

Notas:
-
Usar en un script `callasm` seguido por el offset+1 donde insertaron el código.

En el archivo `linker.ld`, así como en `main.s` cambiar `0x08F90000` por un offset con suficiente espacio libre.

Si se ha repunteado la tabla `gbasestats`, será necesario actualizar el puntero `0x08254784` en `BPRE.ld`

En `BPRE.ld` cambiar los offset  de los siguientes secursos, según convenga:

 gEvIvTILE = 0x08f8fc00;
 gEvIvRAW = 0x08f8fa00;
 gEvIvPAL = 0x08f8ffe0;
