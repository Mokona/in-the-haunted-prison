#ifndef JEU_LYNX_MAIN_CHAR_H
#define JEU_LYNX_MAIN_CHAR_H

#include <stdbool.h>

#include "characters.h"
#include "vector.h"

void initialize_main_char();
void main_char_teleport(unsigned char position, bool forget_old_position);
void main_char_move(unsigned char key,
                    bool (*can_move_to)(unsigned char),
                    void (*commit_move)(unsigned char));
void main_char_display(const unsigned char* (*get_tile)(unsigned char) );
bool is_main_char_at(unsigned char position);
void main_char_cancel_last_move();
unsigned char get_main_char_position();

void main_char_display_stats();
const battle_stats_t* get_main_char_stats();
bool is_main_char_dead();

void main_char_change_weapon(int weapon_id);


void main_char_apply_damage(unsigned char hp_change);
void main_char_heal(unsigned char hp_change);

#endif
