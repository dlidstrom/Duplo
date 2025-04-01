cmake_minimum_required(VERSION 3.15.5)
project(duplo)

if(CMAKE_CONFIGURATION_TYPES)
    list(APPEND CMAKE_CONFIGURATION_TYPES ASAN TSAN UBSAN PROFILE)
    list(REMOVE_DUPLICATES CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
        "Add the configurations that we need"
        FORCE)
endif()


file(GLOB SOURCES src/*.cpp)

SET(DUPLO_VERSION "\"v1.0.1\"" CACHE STRING "Duplo version")

include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
  GIT_SHALLOW 1
  OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(duplo ${SOURCES})

set_target_properties(duplo PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_definitions(duplo PRIVATE DUPLO_VERSION=${DUPLO_VERSION})
target_include_directories(duplo PRIVATE src/include/)
find_package(nlohmann_json 3.11.3 REQUIRED)
target_link_libraries(duplo PRIVATE nlohmann_json::nlohmann_json)

if(NOT MSVC)
    target_compile_options(duplo PRIVATE -Wall -Wextra -pedantic -Werror)
    # https://clang.llvm.org/docs/AddressSanitizer.html
    target_compile_options(duplo PRIVATE $<$<CONFIG:ASAN>:-fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -g -O1>)
    target_link_options(duplo PRIVATE $<$<CONFIG:ASAN>:-fsanitize=address>)
    # https://clang.llvm.org/docs/ThreadSanitizer.html
    target_compile_options(duplo PRIVATE $<$<CONFIG:TSAN>:-fsanitize=thread -g -O1>)
    target_link_options(duplo PRIVATE $<$<CONFIG:TSAN>:-fsanitize=thread>)
    # https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
    target_compile_options(duplo PRIVATE $<$<CONFIG:UBSAN>:-fsanitize=undefined,bounds -g -O1>)
    target_link_options(duplo PRIVATE $<$<CONFIG:UBSAN>:-fsanitize=undefined,bounds>)
    # gperftools CPU Profiler https://github.com/gperftools/gperftools
    target_compile_options(duplo PRIVATE $<$<CONFIG:PROFILE>:-g -O1>)
    target_link_options(duplo PRIVATE $<$<CONFIG:PROFILE>:-lprofiler>)
endif()
