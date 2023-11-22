    PUBLIC  _lynx_clear_screen

    INCLUDE "color_macros.asm"

    ; Clears the screen by writing 0 to all color banks
_lynx_clear_screen:
    ENABLE_ALL_COLORS
    ld      bc,$2000    ; From $1fff to $0000 (mirror of $a000 to $bfff)
    ld      hl,$C000    ; From $c000 to $dfff
clear_screen_loop:
    xor     a
    dec     bc
    ld      (bc),a
    ld      (hl),a
    inc     hl
    ld      a,b
    or      c
    jr      nz,clear_screen_loop
    DISABLE_ALL_COLORS
    ret
