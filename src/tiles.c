#include "tiles.h"

#include "../generated/tile_properties.h"
#include "../generated/tiles_data.h"

const unsigned char* get_tile_graph(unsigned char tile_id)
{
    return all_tiles[tile_id];
}

unsigned char get_tile_property(unsigned char tile_id)
{
    return tile_properties[tile_id];
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
