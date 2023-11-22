#ifndef JEU_LYNX_ROOM_H
#define JEU_LYNX_ROOM_H

#include "room_type.h"
#include "vector.h"

#include <stdbool.h>

void initialize_rooms();

void change_room(room_id_t room_id);
room_id_t get_current_room_id();
void display_room();
unsigned char get_room_tile_property_at(unsigned char position);
const unsigned char* get_room_tile_at(unsigned char position);
room_id_t get_neighbour_room_id(room_id_t id, unsigned char* position);

bool property_is_blocking(unsigned char property);

void position_to_x_y(unsigned char position, char* x, char* y);
unsigned char x_y_to_position(char x, char y);

unsigned char move_position_with_direction(unsigned char position, EMoveDirection direction);
#endif