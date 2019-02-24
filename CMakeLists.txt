cmake_minimum_required(VERSION 3.13)
project(sfml_cardioid)


## === INCLUDE HEADER FILES ===
## -- for SFML --
if (WIN32) # in Windows include directories from SFML/include
    include_directories("$ENV{ProgramW6432}/SFML/include")
endif (WIN32)
if (APPLE)
    # custom Apple CMake goes here
endif (APPLE)
## -- // --

## === SET VARIABLES ===
## -- for SFML --
if (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE) # if you want to link SFML statically (for Windows)
    set(SFML_DIR "$ENV{ProgramW6432}/SFML/lib/cmake/SFML") # In most cases better set in the CMake cache
endif (WIN32)
if (APPLE)
    # custom Apple CMake goes here
endif (APPLE)
## -- // --

## === INCLUDE PACKAGES ===
## include SFML packages (must be in Windows PATH variable!)
find_package(SFML 2.5 COMPONENTS system window graphics audio network REQUIRED)
## -- // --


set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak -g ")

add_executable(sfml_fun main.cpp)


## === LINK LIBRARIES TO EXECUTABLES ===
## -- SFML libraries --
target_link_libraries(sfml_fun sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
## -- // --

## === COPY LIBRARIES ===
## -- SFML libraries --
if (WIN32) # Copy .dll Files to Working Directory (for Windows)
    target_include_directories(sfml_fun PUBLIC "$ENV{ProgramW6432}/SFML/lib")
endif (WIN32)
if (APPLE)
    # custom Apple CMake goes here
endif (APPLE)
## -- // --
