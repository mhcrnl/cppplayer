# cppplayer

Reproductor de música sencillo pensado para uso personal.

El programa usa las librerías mpg123, sfml, y boost-filesystem. En un debian 8 se pueden instalar con el siguiente comando: 

**# apt-get install libmpg123-dev libsfml-dev libboost-filesystem-dev**  
**# apt-get ccache**

Este último no es necesario pero es útil si se va a compilar múltiples veces.

* Solo soporta los formatos mp3, ogg, flac y wav. 
* Solo tiene opciones para pausar la música, salir del programa, o pasar a la siguiente canción.
* Solo funciona en linux aunque puede hacerse funcionar fácilmente en windows.
* Genera una lista aleatoria de todas las canciones que encuentre y las reproduce
* Puede ejecutarse como un solo cliente o cliente/servidor

Para ejecutarlo en modo cliente se le tiene que pasar un argumento el cual 
debe ser el directorio donde se encuentren las canciones. 
Presionando la *q/Q* se sale del programa, presionando *p/P* se pause, *<* para saltar a la canción anterior y *n/N* se salta a la siguiente cancion
Por ejemplo
> player++ ~/Music

Para ejecutarlo como servidor se le debe pasar un segundo argumento "d" (de daemon)
Por ejemplo
> player++ ~/Music d

Para ejecutarlo en modo cliente, el primer parámetro a de ser la opción
Por ejemplo
> player++ p          #pausara la canción 

> player++ n          #saltara a la siguiente canción 

> player++ q          #cerrara el programa 

> player++ <          #saltar a la canción anterior


