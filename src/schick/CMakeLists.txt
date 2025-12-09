# Copyright 2025 The BrightEyes Authors
# Use of this source code is governed by a GPL 3.0 license that can be
# found in the copyright file.

cmake_minimum_required(VERSION 3.15)
project(schick VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find required packages
find_package(SDL2 REQUIRED)

# Define source files
set(SCHICK_SOURCES
    datseg.cpp
    seg001.cpp
    seg002.cpp
    seg003.cpp
    seg004.cpp
    seg005.cpp
    seg006.cpp
    seg007.cpp
    seg008.cpp
    seg009.cpp
    seg010.cpp
    seg011.cpp
    seg024.cpp
    seg025.cpp
    seg026.cpp
    seg027.cpp
    seg028.cpp
    seg029.cpp
    seg030.cpp
    seg031.cpp
    seg032.cpp
    seg033.cpp
    seg034.cpp
    seg035.cpp
    seg036.cpp
    seg037.cpp
    seg038.cpp
    seg039.cpp
    seg040.cpp
    seg041.cpp
    seg042.cpp
    seg043.cpp
    seg044.cpp
    seg045.cpp
    seg046.cpp
    seg047.cpp
    seg048.cpp
    seg049.cpp
    seg050.cpp
    seg051.cpp
    seg052.cpp
    seg053.cpp
    seg054.cpp
    seg055.cpp
    seg056.cpp
    seg057.cpp
    seg058.cpp
    seg059.cpp
    seg060.cpp
    seg061.cpp
    seg062.cpp
    seg063.cpp
    seg064.cpp
    seg065.cpp
    seg066.cpp
    seg067.cpp
    seg068.cpp
    seg069.cpp
    seg070.cpp
    seg071.cpp
    seg072.cpp
    seg073.cpp
    seg074.cpp
    seg075.cpp
    seg076.cpp
    seg077.cpp
    seg078.cpp
    seg079.cpp
    seg080.cpp
    seg081.cpp
    seg082.cpp
    seg083.cpp
    seg084.cpp
    seg085.cpp
    seg086.cpp
    seg087.cpp
    seg088.cpp
    seg089.cpp
    seg090.cpp
    seg091.cpp
    seg092.cpp
    seg093.cpp
    seg094.cpp
    seg095.cpp
    seg096.cpp
    seg097.cpp
    seg098.cpp
    seg099.cpp
    seg100.cpp
    seg101.cpp
    seg102.cpp
    seg103.cpp
    seg104.cpp
    seg105.cpp
    seg106.cpp
    seg107.cpp
    seg108.cpp
    seg109.cpp
    seg110.cpp
    seg111.cpp
    seg112.cpp
    seg113.cpp
    seg114.cpp
    seg115.cpp
    seg116.cpp
    seg117.cpp
    seg118.cpp
    seg119.cpp
    seg120.cpp
    seg121.cpp
    seg122.cpp
    vgalib.cpp
)

# Define header files
set(SCHICK_HEADERS
    common.h
    datseg.h
    hero.h
    seg001.h
    seg002.h
    seg003.h
    seg004.h
    seg005.h
    seg006.h
    seg007.h
    seg008.h
    seg009.h
    seg010.h
    seg011.h
    seg013.h
    seg024.h
    seg025.h
    seg026.h
    seg027.h
    seg028.h
    seg029.h
    seg030.h
    seg031.h
    seg032.h
    seg033.h
    seg034.h
    seg035.h
    seg036.h
    seg037.h
    seg038.h
    seg039.h
    seg040.h
    seg041.h
    seg042.h
    seg043.h
    seg044.h
    seg045.h
    seg046.h
    seg047.h
    seg048.h
    seg049.h
    seg050.h
    seg051.h
    seg052.h
    seg053.h
    seg054.h
    seg055.h
    seg056.h
    seg057.h
    seg058.h
    seg059.h
    seg060.h
    seg061.h
    seg062.h
    seg063.h
    seg064.h
    seg065.h
    seg066.h
    seg067.h
    seg068.h
    seg069.h
    seg070.h
    seg071.h
    seg072.h
    seg073.h
    seg074.h
    seg075.h
    seg076.h
    seg077.h
    seg078.h
    seg079.h
    seg080.h
    seg081.h
    seg082.h
    seg083.h
    seg084.h
    seg085.h
    seg086.h
    seg087.h
    seg088.h
    seg089.h
    seg090.h
    seg091.h
    seg092.h
    seg093.h
    seg094.h
    seg095.h
    seg096.h
    seg097.h
    seg098.h
    seg099.h
    seg100.h
    seg101.h
    seg102.h
    seg103.h
    seg104.h
    seg105.h
    seg106.h
    seg107.h
    seg108.h
    seg109.h
    seg110.h
    seg111.h
    seg112.h
    seg113.h
    seg114.h
    seg115.h
    seg116.h
    seg117.h
    seg118.h
    seg119.h
    seg120.h
    seg121.h
    seg122.h
    symbols.h
    v302de.h
    vgalib.h
    ./AIL/AIL.H
)

# Create executable
add_executable(schick ${SCHICK_SOURCES} ${SCHICK_HEADERS})

if (MSVC)
# so far no special setting beside default for Debug/Release
target_compile_definitions(schick PUBLIC _CRT_SECURE_NO_WARNINGS)
target_compile_options(schick PRIVATE 
   # /fsanitize=address
)
else()
# Compiler flags matching the original Makefile
target_compile_options(schick PRIVATE
    -Og                           # Optimize for debugging
    -g                            # Generate debug info
    -fno-asm                      # Disable inline assembly
    -Wno-register                 # Disable register keyword warnings
    # -fsanitize=address            # Enable AddressSanitizer
    -fno-omit-frame-pointer       # Keep frame pointers for better debugging
)

# Special flags for seg011.cpp
set_source_files_properties(seg011.cpp PROPERTIES
    COMPILE_FLAGS "-Wno-unused-parameter"
)
# Linker flags
# target_link_options(schick PRIVATE
#     -fsanitize=address            # Link with AddressSanitizer
# )
endif()

target_link_libraries(schick PRIVATE SDL2::SDL2main)
target_link_libraries(schick PRIVATE SDL2::SDL2)

# Set output name based on compiler (matching original Makefile behavior)
set_target_properties(schick PROPERTIES
    OUTPUT_NAME "schick_${CMAKE_CXX_COMPILER_ID}"
)

# Installation rules
install(TARGETS schick
    RUNTIME DESTINATION bin
)

# Print configuration summary
message(STATUS "")
message(STATUS "═══════════════════════════════════════════")
message(STATUS "  Schick Configuration Summary")
message(STATUS "═══════════════════════════════════════════")
message(STATUS "  CMake version:    ${CMAKE_VERSION}")
message(STATUS "  C++ Compiler:     ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Build type:       ${CMAKE_BUILD_TYPE}")
message(STATUS "  SDL2 found:       ${SDL2_FOUND}")
message(STATUS "  Output name:      schick_${CMAKE_CXX_COMPILER_ID}")
message(STATUS "═══════════════════════════════════════════")
message(STATUS "")
