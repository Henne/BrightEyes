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

# Create executable
add_executable(schick ${SCHICK_SOURCES})

# Include directories
target_include_directories(schick PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/AIL
    ${SDL2_INCLUDE_DIRS}
)

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
    COMPILE_FLAGS "-Wno-unused-parameters"
)

# Linker flags
# target_link_options(schick PRIVATE
#     -fsanitize=address            # Link with AddressSanitizer
# )

# Link libraries
target_link_libraries(schick PRIVATE
    ${SDL2_LIBRARIES}
)

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
