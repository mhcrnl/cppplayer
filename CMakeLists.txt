cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project (cppplayer)

include_directories(include) 
add_executable(dplayer++ src/main.cpp src/config.cpp src/database.cpp src/mp3.cpp src/manager.cpp src/music.cpp src/musiclist.cpp src/song.cpp)
target_compile_features(dplayer++ PUBLIC cxx_range_for cxx_auto_type cxx_lambdas cxx_aggregate_default_initializers)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
set( CMAKE_EXE_LINKER_FLAGS  "-pthread -lsfml-system -lsfml-audio -lmpg123 -lboost_system -lboost_filesystem -lboost_program_options -ltag -lsqlite3" )

