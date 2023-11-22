    ; Uses BC and A
ENABLE_ALL_COLORS:  MACRO
    ld      a,$07       ; 0x07 = 0000 0111
    ld      bc,$FFFF    ; = write bank 2, 3, read 0, 1
    out     (c),a
    ld      a,$20       ; 0x20 = 0010 0000
    ld      bc,$0080    ; = no mask, green, enable CPU write
    out     (c),a
    ENDM

    ; Uses BC and A
ENABLE_GREEN:  MACRO
    ld      a,$05       ; 0x05 = 0000 0101
    ld      bc,$FFFF    ; = write bank 3, read 0, 1
    out     (c),a
    ld      a,$24       ; 0x24 = 0010 0100
    ld      bc,$0080    ; = mask red/blue, enable CPU write
    out     (c),a
    ENDM

    ; Uses BC and A
ENABLE_RED_BLUE:  MACRO
    ld      a,$03       ; 0x03 = 0000 0011
    ld      bc,$FFFF    ; = write bank 2, read 0, 1
    out     (c),a
    ld      a,$28       ; 0x28 = 0010 1000
    ld      bc,$0080    ; = mask green, enable CPU write
    out     (c),a
    ENDM

    ; Uses BC and A
DISABLE_ALL_COLORS: MACRO
    ld      a,$00       ; 0x00 = 0000 0000
    ld      bc,$FFFF    ; = write bank 1, read 0, 1
    out     (c),a
    ld      bc,$0080    ; = write to bank 1, disable CPU write
    out     (c),a
    ENDM
