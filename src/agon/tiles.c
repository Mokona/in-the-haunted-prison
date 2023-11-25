#include "../tiles.h"
#include <sys/types.h>

#include "../agon/vdp.h"

#include "../../generated/tiles_data.h"

extern void my_agon_putc(unsigned char c) __z88dk_fastcall;

#define FIRST_TILE_ID 80

void push_color(unsigned char red, unsigned char green, unsigned char blue)
{
    for (size_t column = 0; column < 8; column++)
    {
        const unsigned char green_pixel_color = green & 0x80 ? 0xFF : 0x00;
        green <<= 1;
        my_agon_putc(green_pixel_color);

        const unsigned char blue_pixel_color = blue & 0x80 ? 0xFF : 0x00;
        blue <<= 1;
        my_agon_putc(blue_pixel_color);

        const unsigned char red_pixel_color = red & 0x80 ? 0xFF : 0x00;
        red <<= 1;
        my_agon_putc(red_pixel_color);

        my_agon_putc(0xFF);
    }
}

void load_tile(unsigned char tile_id)
{
    bitmap_select(FIRST_TILE_ID + tile_id);
    bitmap_col_load(16, 16);

    const unsigned char* color_pointer = all_tiles[tile_id];

    // Probably very slow, but first implementation
    for (size_t color_index = 0; color_index < 32; color_index++)
    {
        unsigned char red = *color_pointer;
        unsigned char green = *(color_pointer + 32);
        unsigned char blue = *(color_pointer + 64);

        color_pointer += 1;

        push_color(red, green, blue);
    }
}

void initialize_tiles()
{
    for (size_t i = 0; i < tiles_count; i++)
    {
        load_tile(i);
    }
}

void display_tile(unsigned char tile_id, unsigned char x, unsigned char y)
{
    bitmap_select(FIRST_TILE_ID + tile_id);
    bitmap_draw(x * 8, y * 8);
}

void copy_tile_16_at(const unsigned char* sprite, char tile_x, char tile_y)
{
    bitmap_select(FIRST_TILE_ID + 2);
    bitmap_draw(tile_x * 8, tile_y * 8);
}
