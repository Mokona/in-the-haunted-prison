#ifndef JEU_LYNX_TILES_H
#define JEU_LYNX_TILES_H

const unsigned char * get_tile_graph(unsigned char tile_id);
unsigned char get_tile_property(unsigned char tile_id);

void copy_tile_16_at(const unsigned char* sprite, char tile_x, char tile_y);

#endif
