#include "../tiles.h"
#include <sys/types.h>

#include "../agon/vdp.h"

#include "../../generated/tiles_data.h"

extern void my_agon_putc(unsigned char c) __z88dk_fastcall;

#define FIRST_TILE_ID 80

void push_color(unsigned char color)
{
    for (size_t column = 0; column < 8; column++)
    {
        unsigned char pixel_color = color & 0x80 ? 0xFF : 0x00;
        color <<= 1;

        my_agon_putc(pixel_color);
        my_agon_putc(pixel_color);
        my_agon_putc(pixel_color);
        my_agon_putc(0xFF);
    }
}

void load_tile(unsigned char tile_id)
{
    bitmap_select(FIRST_TILE_ID + tile_id);
    bitmap_col_load(16, 16);

    const unsigned char* sprite = all_tiles[tile_id];

    // Probably very slow, but first implementation
    for (size_t line = 0; line < 16; line++)
    {
        unsigned char index = line << 1;

        push_color(sprite[index]);
        push_color(sprite[index + 1]);
    }
}

void initialize_tiles()
{
    for (size_t i = 0; i < tiles_count; i++)
    {
        load_tile(i);
    }
}

const unsigned char* get_tile_graph(unsigned char tile_id)
{
    return all_tiles[tile_id];
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
