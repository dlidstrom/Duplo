cmake_minimum_required(VERSION 3.15.5)
project(duplo)
file(GLOB SOURCES src/*.cpp)

SET(DUPLO_VERSION "\"v1.0.0\"" CACHE STRING "Duplo version")

if(MSVC)
else()
    add_compile_options(-O3 -Wall -Werror -std=c++17)
    add_compile_definitions(DUPLO_VERSION=${DUPLO_VERSION})
endif()
add_executable(duplo ${SOURCES})
