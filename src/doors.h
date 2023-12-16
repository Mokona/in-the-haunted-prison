#ifndef JEU_LYNX_DOORS_H
#define JEU_LYNX_DOORS_H

#include "room_type.h"
#include <stdbool.h>

void initialize_doors();
bool is_door_locked(unsigned char position);
void change_door_or_lever_status(unsigned char id, bool open, bool act_on_lever);
void patch_level_with_doors(room_id_t id, unsigned char* data);


#endif
