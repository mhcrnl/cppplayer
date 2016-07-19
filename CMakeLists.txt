cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project (cppplayer)

#spdlog defines
ADD_DEFINITIONS(-DSPDLOG_NO_DATETIME -DSPDLOG_NO_THREAD_ID -DSPDLOG_NO_NAME)

#Compiler define
option(_NAMED_PIPE "Use named pipes as a comunnication protocol" ON)
IF (_NAMED_PIPE)
    ADD_DEFINITIONS(-D_NAMED_PIPE)
ENDIF()

option(_TCP_SOCKET "Use  a tcp socket for the comunnication" ON)
IF (_TCP_SOCKET)
    ADD_DEFINITIONS(-D_TCP_SOCKET)
ENDIF()

option(_REMOTE_MODE "Run the daemon as a server" OFF)
IF (_REMOTE_MODE)
    ADD_DEFINITIONS(-D_REMOTE_MODE)
ENDIF()

include_directories(include) 
file(GLOB_RECURSE SRC_FILES src/*.cpp)
 
add_executable(dplayer++ ${SRC_FILES})
install(PROGRAMS build/dplayer++ DESTINATION /usr/local/bin)
install(DIRECTORY DESTINATION /usr/include/cppplayer)
install(FILES include/commands.h DESTINATION /usr/include/cppplayer)

target_compile_features(dplayer++ PUBLIC cxx_range_for cxx_auto_type cxx_lambdas)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
#Check if boost exists
find_package(Boost COMPONENTS filesystem) 

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

target_link_libraries(dplayer++ sfml-system sfml-audio mpg123 tag sqlite3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

#RELEASE
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}  -flto")

#DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -Og -DDEBUG")

#THREAD SANITIZER
set(CMAKE_CXX_FLAGS_THREAD "${CMAKE_CXX_FLAGS_THREAD} -fsanitize=thread -DDEBUG -Og -ggdb")
set(CMAKE_EXE_LINKER_FLAGS_THREAD "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")

#ADDRESS SANITIZER
set(CMAKE_CXX_FLAGS_ADDRESS "${CMAKE_CXX_FLAGS_ADDRESS} -fsanitize=address -DDEBUG -Og -ggdb")
set(CMAKE_EXE_LINKER_FLAGS_ADDRESS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

