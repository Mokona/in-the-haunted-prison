    public  _vdp_init
    public _my_agon_putc

_vdp_init:
    push    ix

    lea     hl, ix+5
    ld.lil  (keycode_ptr),hl
    pop     ix
    ret

keycode_ptr:
    defb    0, 0, 0


_my_agon_putc:
    ld      a, l
    defb    $49     ; LIS
    rst     $10
    ret
