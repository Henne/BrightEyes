cmake_minimum_required(VERSION 3.25.1)
project(NGen VERSION 1.0.0
    DESCRIPTION "DSA1/RAK1 Character generator"
    HOMEPAGE_URL "https://github.com/Henne/BrightEyes"
    LANGUAGES C)

set(CMAKE_C_STANDARD 11)
if (MSVC)
# so far no special setting beside default for Debug/Release
else()
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g -fno-asm")
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Workaround for including either <SDL2/SDL.h> or <SDL.h>
get_target_property(SDL2_INCLUDE_DIR SDL2::SDL2 INTERFACE_INCLUDE_DIRECTORIES)
cmake_path(GET SDL2_INCLUDE_DIR PARENT_PATH SDL2_INCLUDE_DIR)
get_target_property(SDL2_mixer_INCLUDE_DIR SDL2_mixer::SDL2_mixer INTERFACE_INCLUDE_DIRECTORIES)
cmake_path(GET SDL2_mixer_INCLUDE_DIR PARENT_PATH SDL2_mixer_INCLUDE_DIR)

add_executable(ngen
    cda_code.h
    cda_code.c
    ngen.c
    powerp20.c
    vgalib.h
    vgalib.c
    ail_stub.c
    hero.h
)

# Rename output to include compiler
get_filename_component(COMPILER_NAME ${CMAKE_C_COMPILER_ID} NAME)
set_target_properties(ngen PROPERTIES OUTPUT_NAME "ngen_${COMPILER_NAME}")

find_package(OpenMP)
if(OpenMP_C_FOUND)
  target_link_libraries(ngen PUBLIC OpenMP::OpenMP_C)
endif()

# On Windows copy SDL2 DLLs next to the executable
if(WIN32)
  add_custom_command(TARGET ngen POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:ngen>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "$<TARGET_FILE:SDL2_mixer::SDL2_mixer>" "$<TARGET_FILE_DIR:ngen>")
endif()

target_include_directories(ngen PRIVATE ${SDL2_INCLUDE_DIR})
target_link_libraries(ngen PUBLIC SDL2::SDL2 SDL2_mixer::SDL2_mixer SDL2::SDL2main)
install(TARGETS ngen RUNTIME DESTINATION bin)
