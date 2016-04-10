# cppplayer

Reproductor de música sencillo pensado para uso personal.

El programa usa las librerías mpg123, sfml, TagLib boost-program-options y boost-filesystem. En un debian 8 se pueden instalar con el siguiente comando: 

**# apt-get install libmpg123-dev libsfml-dev libboost-filesystem-dev libboost-program-options-dev libtag1-dev**  
**# apt-get install ccache**

Este último no es necesario pero es útil si se va a compilar múltiples veces.

* Solo soporta los formatos mp3, ogg, flac y wav. 
* Solo funciona en linux aunque puede hacerse funcionar fácilmente en windows.
* Genera una lista aleatoria de todas las canciones que encuentre y las reproduce

En el archivo **player++.config.example** hay una configuración de ejemplo. Para modificar la configuración se debe editar el fichero **~/.config/player++**

En la carpeta "client_console" en el archivo main.cpp se pueden ver los comandos usables

### Siguientes cosas a hacer
+ Mejorar el sistema de cache
+ Filtrar por artista
+ Mejorar el sistema de randomización
+ Crear lista de favoritos
+ Guardar una estructura de todas las canciones para agilizar las busquedas
