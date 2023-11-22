#ifndef JEU_LYNX_SPAWNERS_H
#define JEU_LYNX_SPAWNERS_H

#include "room_type.h"

// Parameters are Room ID, Position and Spawner ID
typedef void (*spawner_callback_t)(room_id_t, unsigned char, unsigned char);

unsigned char get_main_character_spawn_position(unsigned char* room_id);
void iterate_spawners_for_room(room_id_t room_id, spawner_callback_t callback);

#endif
