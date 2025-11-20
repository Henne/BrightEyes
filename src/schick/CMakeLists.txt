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
find_package(OpenMP)

# Define source files
set(SCHICK_SOURCES
    # Core systems
    datseg.cpp
    cdrom.cpp
    misc.cpp
    movement.cpp
    graphics.cpp
    fight_core.cpp
    fight_base.cpp
    random_dice.cpp
    rasterlib.cpp
    pp20_decompress.cpp
    ems.cpp
    sound_ail.cpp

    # Game data & core mechanics
    diary.cpp
    locations.cpp
    save_text.cpp
    file_loader.cpp
    resource_loader.cpp
    playmask.cpp
    talk_date.cpp
    tavern_helpers.cpp
    fight_helpers.cpp

    # Fight system
    fight_menu.cpp
    fight_system.cpp
    fight_logic.cpp
    fight_auto_hero.cpp
    fight_enemy_attack.cpp
    fight_actions.cpp
    fight_mechanics.cpp
    fight_scenarios.cpp
    fight_control.cpp
    fight_hero_action.cpp
    fight_enemy_action.cpp
    fight_anim_figures.cpp
    fight_anim_effects.cpp
    status_display.cpp
    heroes_group.cpp
    status_menu.cpp
    group_mgmt.cpp
    level_up.cpp

    # Locations & environments
    wilderness_camp.cpp
    area_camp.cpp
    location_healer.cpp
    location_inn.cpp
    merchant_main.cpp
    merchant_buy.cpp
    merchant_sell.cpp
    location_smith.cpp
    tavern_main.cpp
    tavern_talk.cpp
    temple_main.cpp
    temple_miracle.cpp
    harbor_main.cpp
    harbor_helper.cpp
    special_anims.cpp
    town_main.cpp
    city_events.cpp
    thorwal_buildings1.cpp
    thorwal_buildings2.cpp
    phexcaer_buildings1.cpp
    phexcaer_buildings2.cpp
    informer_script.cpp
    tavern_gossip.cpp
    automap.cpp

    # Dungeon systems
    dungeon_common1.cpp
    dungeon_common2.cpp
    dungeon_ship_death.cpp
    dungeon_inn.cpp
    dungeon_spider_cave.cpp
    dungeon_wolf_cave.cpp
    dungeon_cave2.cpp
    dungeon_mageruin.cpp
    dungeon_orc_lair.cpp
    dungeon_main.cpp
    dungeon_cave4.cpp
    dungeon_pirate_cave.cpp
    dungeon_thorwal1.cpp
    dungeon_thorwal2.cpp
    dungeon_ruined_castle.cpp
    dungeon_oberorken_mine.cpp
    dungeon_prem_mine.cpp
    treasures.cpp

    # Game mechanics & magic
    travel_mode1.cpp
    travel_mode2.cpp
    npcs.cpp
    text_output.cpp
    gui.cpp
    magic_main.cpp
    spells1.cpp
    spells2.cpp
    spells3.cpp
    spells_monster.cpp
    talents.cpp
    alchemy_cure.cpp
    inventory_main.cpp
    inventory_misc.cpp
    item_use.cpp
    consume.cpp
    travel_event1.cpp
    travel_event2.cpp
    travel_event3.cpp
    travel_event4.cpp
    travel_event5.cpp
    travel_event6.cpp
    travel_event7.cpp
    travel_event8.cpp
    travel_event9.cpp
    travel_event10.cpp
    disease_effect.cpp
    game_init.cpp
    poison_effect.cpp
    dummy_location.cpp

    # Graphics library
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

# Special flags for sound_ail.cpp
set_source_files_properties(sound_ail.cpp PROPERTIES
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

# Configure OpenMP
if(OpenMP_CXX_FOUND)
    target_link_libraries(schick PRIVATE OpenMP::OpenMP_CXX)
    target_compile_definitions(schick PRIVATE HAVE_OPENMP=1)
    message(STATUS "OpenMP support enabled")
else()
    target_compile_definitions(schick PRIVATE HAVE_OPENMP=0)
    message(STATUS "OpenMP not found - building without OpenMP support")
    message(STATUS "To enable OpenMP on macOS: brew install libomp")
endif()

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
message(STATUS "  OpenMP found:     ${OpenMP_FOUND}")
message(STATUS "  Output name:      schick_${CMAKE_CXX_COMPILER_ID}")
message(STATUS "═══════════════════════════════════════════")
message(STATUS "")
