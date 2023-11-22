#ifndef JEU_LYNX_INVENTORY_H
#define JEU_LYNX_INVENTORY_H

#include <stdbool.h>

void initialize_inventory();
void add_to_inventory(unsigned char tile_id);
void display_inventory();
bool has_secret_been_found();
bool is_in_inventory(unsigned char tile_id);

#endif
