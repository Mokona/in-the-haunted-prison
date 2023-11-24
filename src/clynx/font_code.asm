    PUBLIC _copy_8x8_mono_to_vram

    INCLUDE "color_macros.asm"
    INCLUDE "copy_macros.asm"

    EXTERN _font_data

    ; SP+2/SP+3 = destination offset for Blue address
    ; SP+4/SP+5 = source
_copy_8x8_mono_to_vram:
    ld      hl,2
    add     hl,sp
    ld      e,(hl)
    inc     hl
    ld      d,(hl)      ; DE = destination offset for Blue address

    inc     hl
    ld      a,(hl)      ; HL = source
    inc     hl
    ld      h,(hl)
    ld      l,a

    ; ex      de,hl       ; HL = source, DE = destination offset for Blue address

    ; ld      de,10*32*8
    ; ld      hl,_font_data+8

    ENABLE_RED_BLUE

    ; Blue part
    ld      ix,$A000    ; Blue
    add     ix,de       ; IX = destination blue address
    COPY_LOOP_8

    ld      b,8
dec_1:
    dec     hl
    djnz    dec_1

    ; Red part
    ld      de,$2000-(32*8)
    add     ix,de       ; IX = destination green/red address
    COPY_LOOP_8

    ld      b,8
dec_2:
    dec     hl
    djnz    dec_2

    ; Green part
    ENABLE_GREEN
    ld      de,-(32*8)
    add     ix,de       ; IX = destination green/red address
    COPY_LOOP_8

    DISABLE_ALL_COLORS

    ret
