#ifndef JEU_LYNX_OBJECTS_H
#define JEU_LYNX_OBJECTS_H

#include <stdbool.h>

#include "room_type.h"

// TODO: it was supposed to be private. Lever is using the objects directly. It should not.
typedef struct {
    unsigned char room_id;
    char position;
    unsigned char tile_id;
    unsigned char properties : 7;
    unsigned char picked : 1; // Also activated for lever
} Object;

typedef unsigned char object_id_t;
#define NO_OBJECT 0xFF

void initialize_objects();
void display_objects(room_id_t room_id);
object_id_t get_object_at(room_id_t room_id, unsigned char position);
unsigned char get_object_tile_id(object_id_t object_id);
void act_on_object(object_id_t object_id);
void drop_object(object_id_t object_id);
void change_object_tile_id(object_id_t id, unsigned char tile_id);

#endif
