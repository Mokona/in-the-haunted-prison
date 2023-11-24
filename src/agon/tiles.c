#include "../tiles.h"

#include "../../generated/tiles_data.h"

void initialize_tiles()
{
}

const unsigned char* get_tile_graph(unsigned char tile_id)
{
    return all_tiles[tile_id];
}


void copy_tile_16_at(const unsigned char* sprite, char tile_x, char tile_y)
{
}
