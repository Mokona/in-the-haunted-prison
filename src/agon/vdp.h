#ifndef SNAKE_VDP_H
#define SNAKE_VDP_H

void set_mode(int m);
void scaling(char i);
void color(char c);
void g_color(char mode, char c);
void color_change(char l, char p, char r, char g, char b);
void vdp_plot(char mode, int x, int y);
void move_to(unsigned int x, unsigned int y);
void line(unsigned int x, unsigned int y);
void bitmap_select(unsigned char n);
void bitmap_col_load(int sx, int sy);
void bitmap_draw(int x, int y);
void sprite_select(char n);
void sprite_clear_frames();
void sprite_add_frame(int bitmap_n);
void sprite_activate(int count);
void sprite_frame_select(int n);
void sprite_show();
void sprite_move_to(int x, int y);
void sprite_update();
void cursor_enable(char i);

extern void vdp_init();
extern void vdp_vsync();
extern char vdp_get_vkey();
extern char vdp_get_vkey_pressed();

#endif
