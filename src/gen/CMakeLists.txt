cmake_minimum_required(VERSION 3.25.1)
project(NGen VERSION 1.0.0
    DESCRIPTION "DSA1/RAK1 Character generator"
    HOMEPAGE_URL "https://github.com/Henne/BrightEyes"
    LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g -fno-asm")

find_package(SDL2 REQUIRED)

# Workaround for including either <SDL2/SDL.h> or <SDL.h>
get_target_property(SDL2_INCLUDE_DIR SDL2::SDL2 INTERFACE_INCLUDE_DIRECTORIES)
cmake_path(GET SDL2_INCLUDE_DIR PARENT_PATH SDL2_INCLUDE_DIR)

add_executable(ngen
    cda_code.c
    ngen.c
    powerp20.c
    vgalib.c
    ail_stub.c)

get_filename_component(COMPILER_NAME ${CMAKE_C_COMPILER} NAME)
set_target_properties(ngen PROPERTIES OUTPUT_NAME "ngen_${COMPILER_NAME}")

target_include_directories(ngen PRIVATE ${SDL2_INCLUDE_DIR})
target_link_libraries(ngen PUBLIC SDL2::SDL2)
install(TARGETS ngen RUNTIME DESTINATION bin)
