cmake_minimum_required (VERSION 2.6)
project (cppplayer)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pedantic")
set( CMAKE_EXE_LINKER_FLAGS  "-pthread -lsfml-system -lsfml-audio -lmpg123 -lboost_system -lboost_filesystem -lboost_program_options -ltag -lsqlite3" )

include_directories(include) 
add_executable(dplayer++ src/main.cpp src/config.cpp src/database.cpp src/mp3.cpp src/manager.cpp src/music.cpp src/musiclist.cpp src/song.cpp)
