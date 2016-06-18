# cppplayer

[![Build Status](https://travis-ci.org/stakewinner00/cppplayer.svg?branch=master)](https://travis-ci.org/stakewinner00/cppplayer)

Reproductor de música sencillo que busca la simplicidad y facilidad de configuración (se podría ver como alternativa a mpd)

El programa usa las librerías mpg123, sfml, TagLib boost-program-options y boost-filesystem. 

#### Instalación en Debian/Ubuntu

**# apt-get install libmpg123-dev libsfml-dev libboost-filesystem-dev libboost-program-options-dev libtag1-dev**

Para compilarlo
> mkdir -p build; cd build; cmake ..; make

Nota: En debian stable (jessie) no hay la versión necesaria de cmake, para arreglarlo se puede pasar a debian testing, que si tiene la versión necesaria. 

#### Configuración

El programa al iniciar lee la configuración de ~/.config/player++/daemon.conf 
Si no existe ninguna configuración se crea una por defecto

#### Features

- Avanzar/Retroceder/Pausar/Reproducir etc
- Distintos sorts (actualmente solo funciona el modo aleatorio)
- Distintos filtros (actualmente solo funciona el filtro por artista)
- Fácilmente "scripteable"


#### TODO

- usar libxdg para buscar el directorio por defecto donde buscar cancioens (parsear ~/.config/user-dirs.dirs si existe)
- Playlists
- Base de datos SQL
- Soporte para plugins
- Ecualizador
- Cache de canciones recientes (para agilizar la búsqueda)

#### Colaboradores
Lista de gente que ha colaborado de alguna forma con el proyecto
[@masapastosa](https://github.com/masapastosa) [@javorcd](https://github.com/javorcd)
