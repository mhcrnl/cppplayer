cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project (cppplayer)

include_directories(include) 
file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(dplayer++ ${SRC_FILES})
install(PROGRAMS build/dplayer++ DESTINATION /usr/local/bin)

target_compile_features(dplayer++ PUBLIC cxx_range_for cxx_auto_type cxx_lambdas)


#Check if boost exists
find_package(Boost 1.60.0 COMPONENTS filesystem program_options) 

if(NOT Boost_FOUND)
    message( FATAL_ERROR "Boost libraries not found." )
endif()

include_directories(${Boost_INCLUDE_DIRS}) 
target_link_libraries(dplayer++ ${Boost_LIBRARIES})

#Find thread library
find_package(Threads) 

if(NOT Threads_FOUND)
    message(FATAL_ERROR "This system doesn't support threads.")
endif()

target_link_libraries(dplayer++ ${CMAKE_THREAD_LIBS_INIT})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_EXE_LINKER_FLAGS  "-lsfml-system -lsfml-audio -lmpg123 -ltag -lsqlite3" )

#RELEASE
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  -flto")

#DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -Og -DDEBUG")

#THREAD SANITIZER
set(CMAKE_CXX_FLAGS_THREAD "${CMAKE_CXX_FLAGS_THREAD} -fsanitize=thread -DDEBUG -Og -ggdb")
set(CMAKE_EXE_LINKER_FLAGS_THREAD "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")

#ADDRESS SANITIZER
set(CMAKE_CXX_FLAGS_ADDRESS "${CMAKE_CXX_FLAGS_ADDRESS} -fsanitize=address -DDEBUG -Og -ggdb")
set(CMAKE_EXE_LINKER_FLAGS_THREAD "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

