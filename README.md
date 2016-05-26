# cppplayer

Reproductor de música sencillo que busca la simplicidad y facilidad de configuración (se podría ver como alternativa a mpd)

El programa usa las librerías mpg123, sfml, TagLib boost-program-options y boost-filesystem. 

#### Instalación en Debian/Ubuntu

**# apt-get install libmpg123-dev libsfml-dev libboost-filesystem-dev libboost-program-options-dev libtag1-dev**

Para compilarlo
  mkdir -p build; cd build; cmake ..; make

#### Configuración

El programa al iniciar lee la configuración de ~/.config/player++/daemon.conf

#### Features

- Avanzar/Retroceder/Pausar/Reproducir etc
- Distintos sorts (actualmente solo funciona el modo aleatorio)
- Distintos filtros (actualmente solo funciona el filtro por artista)
- Fácilmente "scripteable"


#### TODO

- Playlists
- Base de datos SQL
- Soporte para plugins
- Ecualizador
- Cache de canciones recientes (para agilizar la búsqueda)
