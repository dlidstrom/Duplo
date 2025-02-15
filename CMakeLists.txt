cmake_minimum_required(VERSION 3.15.5)
project(duplo)
file(GLOB SOURCES src/*.cpp)

SET(DUPLO_VERSION "\"v1.0.1\"" CACHE STRING "Duplo version")

add_executable(duplo ${SOURCES})

set_target_properties(duplo PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_compile_definitions(duplo PRIVATE DUPLO_VERSION=${DUPLO_VERSION})

if(NOT MSVC)
    target_compile_options(duplo PRIVATE -Wall -Werror)
endif()


