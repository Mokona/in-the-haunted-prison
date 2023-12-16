extern void my_agon_putc(char c) __z88dk_fastcall;

void put_word(unsigned int w)
{
    my_agon_putc(w & 0xFF);
    my_agon_putc((w >> 8) & 0xFF);
}

void vdp_plot(char mode, unsigned int x, unsigned int y)
{
    my_agon_putc(25);
    my_agon_putc(mode);
    put_word(x);
    put_word(y);
}

void bitmap_select(unsigned char n)
{
    my_agon_putc(23);
    my_agon_putc(27);
    my_agon_putc(0);
    my_agon_putc(n);
}

void bitmap_col_load(int sx, int sy)
{
    my_agon_putc(23);
    my_agon_putc(27);
    my_agon_putc(1);
    my_agon_putc(sx & 0xFF);
    my_agon_putc((sx >> 8) & 0xFF);
    my_agon_putc(sy & 0xFF);
    my_agon_putc((sy >> 8) & 0xFF);
}

void bitmap_draw(int x, int y)
{
    my_agon_putc(23);
    my_agon_putc(27);
    my_agon_putc(3);
    my_agon_putc(x & 0xFF);
    my_agon_putc((x >> 8) & 0xFF);
    my_agon_putc(y & 0xFF);
    my_agon_putc((y >> 8) & 0xFF);
}

void cursor_enable(char i)
{
    my_agon_putc(23);
    my_agon_putc(1);
    my_agon_putc(i);
}
