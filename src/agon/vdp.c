extern void my_agon_putc(char c) __z88dk_fastcall;


void color(char c)
{
    my_agon_putc(17);
    my_agon_putc(c);
}

void g_color(char mode, char c)
{
    my_agon_putc(18);
    my_agon_putc(mode);
    my_agon_putc(c);
}

void color_change(char l, char p, char r, char g, char b)
{
    my_agon_putc(19);
    my_agon_putc(l);
    my_agon_putc(p);
    my_agon_putc(r);
    my_agon_putc(g);
    my_agon_putc(b);
}

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

void move_to(unsigned int x, unsigned int y)
{
    vdp_plot(4, x, y);
}

void line(int x, int y)
{
    vdp_plot(5, x, y);
}

void bitmap_select(char n)
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

void sprite_select(char n)
{
    my_agon_putc(23);
    my_agon_putc(27);
    my_agon_putc(4);
    my_agon_putc(n);
}

void sprite_clear_frames()
{
    my_agon_putc(23);
    my_agon_putc(27);
    my_agon_putc(5);
}

void sprite_add_frame(int bitmap_n)
{
    my_agon_putc(23);
    my_agon_putc(27);
    my_agon_putc(6);
    my_agon_putc(bitmap_n);
}

void sprite_activate(int count)
{
    my_agon_putc(23);
    my_agon_putc(27);
    my_agon_putc(7);
    my_agon_putc(count);
}

void sprite_frame_select(int n)
{
    my_agon_putc(23);
    my_agon_putc(27);
    my_agon_putc(10);
    my_agon_putc(n);
}

void sprite_show()
{
    my_agon_putc(23);
    my_agon_putc(27);
    my_agon_putc(11);
}

void sprite_move_to(int x, int y)
{
    my_agon_putc(23);
    my_agon_putc(27);
    my_agon_putc(13);
    my_agon_putc(x & 0xFF);
    my_agon_putc((x >> 8) & 0xFF);
    my_agon_putc(y & 0xFF);
    my_agon_putc((y >> 8) & 0xFF);
}

void sprite_update()
{
    my_agon_putc(23);
    my_agon_putc(27);
    my_agon_putc(15);
}

void cursor_enable(char i)
{
    my_agon_putc(23);
    my_agon_putc(1);
    my_agon_putc(i);
}

void vsync()
{
    my_agon_putc(23);
    my_agon_putc(0);
    my_agon_putc(0xc3);
}
