cmake_minimum_required(VERSION 3.26)
project(haunted C ASM)

set(CMAKE_C_STANDARD 11)

set(EXE_NAME haunted)

add_executable(${EXE_NAME}
        src/main.c
        src/vdp.c
        src/mos.asm
)

target_link_options(${EXE_NAME} PRIVATE -pragma-define:CLIB_DEFAULT_SCREEN_MODE=8)
