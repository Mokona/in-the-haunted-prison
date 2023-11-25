#ifndef JEU_LYNX_TILES_H
#define JEU_LYNX_TILES_H

void initialize_tiles();
const unsigned char* get_tile_graph(unsigned char tile_id);
unsigned char get_tile_property(unsigned char tile_id);

void display_tile(unsigned char tile_id, unsigned char x, unsigned char y);

#endif
