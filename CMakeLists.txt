cmake_minimum_required(VERSION 3.26)
project(haunted C ASM)

set(CMAKE_C_STANDARD 11)

set(EXE_NAME haunted)

add_custom_command(
        OUTPUT generated/doors_data.c generated/inventory_data.c generated/level_data.h generated/levers_data.c
        generated/links_data.c generated/objects_data.c generated/spawner_data.c generated/texts_data.c
        generated/tile_properties.c
        COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/export_tiled.py ${CMAKE_BINARY_DIR}
        COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/export_tiled.py ${PROJECT_SOURCE_DIR}
        DEPENDS ${CMAKE_SOURCE_DIR}/tools/export_tiled.py
        DEPENDS ${CMAKE_SOURCE_DIR}/data/tiled/jeu-lynx.tsx
        DEPENDS ${CMAKE_SOURCE_DIR}/data/tiled/jeu-lynx.world
        DEPENDS ${CMAKE_SOURCE_DIR}/data/tiled/jeu-lynx.tiled-project
)

add_custom_command(
        OUTPUT generated/font_data.c generated/font_data.h
        COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/make_font.py ${CMAKE_BINARY_DIR}
        COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/make_font.py ${PROJECT_SOURCE_DIR}
        DEPENDS ${CMAKE_SOURCE_DIR}/tools/make_font.py
        DEPENDS ${CMAKE_SOURCE_DIR}/data/PublicPixel.ttf
)

set(COMMON_FILES
        src/main.c
        src/battle.c
        src/doors.c
        src/inventory.c
        src/levers.c
        src/links.c
        src/loop_main.c
        src/loop_title.c
        src/loop_retry.c
        src/loop_victory.c
        src/memory.c
        src/main_char.c
        src/memory.c
        src/mobs.c
        src/objects.c
        src/print.c
        src/room.c
        src/screen.c
        src/spawners.c
        src/teleport.c
        src/text_zone.c
        src/tiles.c
        src/utils.c
        src/vector.c)

set(GENERATED_FILES
        generated/doors_data.c
        generated/font_data.c
        generated/inventory_data.c
        generated/level_data.h
        generated/levers_data.c
        generated/links_data.c
        generated/objects_data.c
        generated/spawner_data.c
        generated/texts_data.c
        generated/tile_properties.c
        generated/tiles_data.c
)

set(AGON_FILES
        src/agon/vdp.c
        src/agon/mos.asm
        src/agon/font.c
        src/agon/input.c
        src/agon/tiles.c)

set(LYNX_FILES
        src/clynx/check_96k.c
        src/clynx/font.c
        src/clynx/font_code.asm
        src/clynx/screen.asm
        src/clynx/tiles.c
        src/clynx/tiles_code.asm)

if(ZCCTARGET STREQUAL "agon")
    set(SPECIFIC_FILES ${AGON_FILES})
endif()

if(ZCCTARGET STREQUAL "lynx")
    set(SPECIFIC_FILES ${LYNX_FILES})
endif()

add_executable(${EXE_NAME}_fr ${SPECIFIC_FILES} ${COMMON_FILES} ${GENERATED_FILES})
target_link_options(${EXE_NAME}_fr PRIVATE -pragma-define:CLIB_DEFAULT_SCREEN_MODE=8 -create-app -subtarget=default)
target_compile_definitions(${EXE_NAME}_fr PRIVATE LANGUAGE=0)

add_executable(${EXE_NAME}_en ${SPECIFIC_FILES} ${COMMON_FILES} ${GENERATED_FILES})
target_link_options(${EXE_NAME}_en PRIVATE -pragma-define:CLIB_DEFAULT_SCREEN_MODE=8 -create-app -subtarget=default)
target_compile_definitions(${EXE_NAME}_en PRIVATE LANGUAGE=1)

add_executable(${EXE_NAME}_eo ${SPECIFIC_FILES} ${COMMON_FILES} ${GENERATED_FILES})
target_link_options(${EXE_NAME}_eo PRIVATE -pragma-define:CLIB_DEFAULT_SCREEN_MODE=8 -create-app -subtarget=default)
target_compile_definitions(${EXE_NAME}_eo PRIVATE LANGUAGE=2)
