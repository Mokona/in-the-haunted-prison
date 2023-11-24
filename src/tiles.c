#include "tiles.h"

#include "../generated/tile_properties.h"

unsigned char get_tile_property(unsigned char tile_id)
{
    return tile_properties[tile_id];
}
