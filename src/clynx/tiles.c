#include "../tiles.h"

#include "../../generated/tiles_data.h"

void initialize_tiles()
{
}

extern void copy_16x16_to_vram(const unsigned char* sprite, unsigned short destination);

void copy_tile_16_at(const unsigned char* sprite, char tile_x, char tile_y)
{
    unsigned short offset = tile_y;
    offset *= 32;
    offset *= 8;
    offset += tile_x;
    // int offset = ((int)tile_y * 32 * 8) + (int)tile_x;
    // struct sprite_with_destination params = {sprite, offset};
    copy_16x16_to_vram(sprite, offset);
}
