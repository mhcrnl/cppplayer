cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project (cppplayer)

include_directories(include) 
file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(dplayer++ ${SRC_FILES})

target_compile_features(dplayer++ PUBLIC cxx_range_for cxx_auto_type cxx_lambdas cxx_aggregate_default_initializers)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_EXE_LINKER_FLAGS  "-pthread -lsfml-system -lsfml-audio -lmpg123 -ltag -lsqlite3" )

#Check if boost exists
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.60.0 COMPONENTS filesystem program_options ) 

if(NOT Boost_FOUND)
    message( FATAL_ERROR "Boost libraries not found." )
endif()

include_directories(${Boost_INCLUDE_DIRS}) 
target_link_libraries(dplayer++ ${Boost_LIBRARIES})


#RELEASE
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto")

#DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -Og -DDEBUG")

#THREAD SANITIZER
set(CMAKE_BUILD_TYPE thread)
set(CMAKE_CXX_FLAGS_THREAD "-fsanitize=thread")
set(CMAKE_EXE_LINKER_FLAGS_THREAD "-fsanitize=thread")

#ADDRESS SANITIZER
set(CMAKE_BUILD_TYPE address)
set(CMAKE_CXX_FLAGS_THREAD "-fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS_THREAD "-fsanitize=address")




