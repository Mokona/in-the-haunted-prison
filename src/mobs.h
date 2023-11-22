#ifndef JEU_LYNX_MOBS_H
#define JEU_LYNX_MOBS_H

#include "characters.h"
#include "room_type.h"
#include "vector.h"

#include <stdbool.h>

#define NO_MOB 0xFF

typedef unsigned char mob_id_t;

void initialize_mob_graphs();
void initialize_mobs();
void clear_game();
void activate_mob_for_room(room_id_t room_id);
void display_mobs(room_id_t room_id, const unsigned char* (*get_tile)(unsigned char) );
void mobs_move(room_id_t room_id,
               bool (*can_move_to)(unsigned char),
               unsigned char attractor_position);
void mobs_clean();
mob_id_t get_mob_at(room_id_t room_id, unsigned char position);
void kill_mob(mob_id_t mob_id);

const battle_stats_t* get_mob_stats(mob_id_t id);
void mob_apply_damage(mob_id_t id, unsigned char hp_change);

#endif
