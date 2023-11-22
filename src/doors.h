#ifndef JEU_LYNX_DOORS_H
#define JEU_LYNX_DOORS_H

#include "room_type.h"
#include <stdbool.h>

void initialize_doors();
bool is_door_locked(unsigned char position);
void open_door_with_key(unsigned char id);
void change_lever_door_status(unsigned char lever_id, bool open);
void patch_level_with_doors(room_id_t id, unsigned char* data);


#endif
