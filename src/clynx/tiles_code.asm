    PUBLIC  _copy_8x8_to_vram
    PUBLIC  _copy_16x16_to_vram

    INCLUDE "color_macros.asm"
    INCLUDE "copy_macros.asm"
    ; INCLUDE "../generated/tiles_data.asm"

    ; HL = source
    ; DE = destination offset for Blue address
_copy_8x8_to_vram:
    ENABLE_RED_BLUE

    ; Blue part
    ld      ix,$A000    ; Blue
    add     ix,de       ; IX = destination blue address
    COPY_LOOP_8

    ; Red part
    ld      de,$2000-(32*8)
    add     ix,de       ; IX = destination green/red address
    COPY_LOOP_8

    ; Green part
    ENABLE_GREEN
    ld      de,-(32*8)
    add     ix,de       ; IX = destination green/red address
    COPY_LOOP_8

    DISABLE_ALL_COLORS

    ret


COPY_LOOP_16: MACRO
    LOCAL   loop

    ld      b,16        ; 16 rows
    ld      de,31
loop:
    ld      a,(hl)      ; Get 8 pixels / 1 byte
    ld      (ix),a      ; Copy to VRAM
    inc     ix          ; Next byte
    inc     hl
    ld      a,(hl)      ; Get 8 pixels / 1 byte again
    ld      (ix),a      ; Copy to VRAM
    add     ix,de       ; Next destination row
    inc     hl          ; Next source row
    djnz    loop

    ENDM

    ; SP+4/SP+5 = source
    ; SP+6/SP+7 = destination offset for Blue address
_copy_16x16_to_vram:
    ld      hl,4
    add     hl,sp
    ld      e,(hl)
    inc     hl
    ld      d,(hl)      ; DE = source

    inc     hl
    ld      a,(hl)     ; HL = destination offset for Blue address
    inc     hl
    ld      h,(hl)
    ld      l,a

    ex      de,hl       ; HL = source, DE = destination offset for Blue address

    ENABLE_RED_BLUE

    ; Blue part
    ld      ix,$A000    ; Blue
    add     ix,de       ; IX = destination blue address
    COPY_LOOP_16

    ; Red part
    ld      de,$2000-(32*16)
    add     ix,de       ; IX = destination green/red address
    COPY_LOOP_16

    ; Green part
    ENABLE_GREEN
    ld      de,-(32*16)
    add     ix,de       ; IX = destination green/red address
    COPY_LOOP_16

    DISABLE_ALL_COLORS

    ret
