COPY_LOOP_8: MACRO
    LOCAL   loop

    ld      b,8         ; 8 rows
    ld      de,32
loop:
    ld      a,(hl)      ; Get 8 pixels / 1 byte
    ld      (ix),a      ; Copy to VRAM
    add     ix,de       ; Next destination row
    inc     hl          ; Next source row
    djnz    loop

    ENDM

