#include "levers.h"

#include "../generated/levers_data.h"
#include "../generated/objects_data.h"
#include "doors.h"
#include "objects.h"

#include "room.h"

typedef struct {
    unsigned char room_id;
    char position;
    unsigned char lever_id;
} Lever;

const Lever* get_lever_at(unsigned char room_id, unsigned char position)
{
    const Lever* all_levers = (Lever*) levers;

    for (size_t i = 0; i < levers_count; i++)
    {
        const Lever* lever = &all_levers[i];
        if (lever->room_id == room_id)
        {
            if (lever->position == position)
            {
                return lever;
            }
        }
    }

    return NULL;
}

void initialize_levers()
{
    // Iterate on all objects that are levers
    Object* all_objects = (Object*) objects;
    for (size_t i = 0; i < objects_count; i++)
    {
        Object* object = &all_objects[i];
        if (object->properties & E_LEVER)
        {
            const Lever* lever = get_lever_at(object->room_id, object->position);
            object->picked = (object->tile_id == lever_activated_tile_id);
            change_lever_door_status(lever->lever_id, object->picked);
        }
    }
}

void act_on_lever(Object* object)
{
    const Lever* lever = get_lever_at(object->room_id, object->position);
    const unsigned char lever_id = lever->lever_id;
    change_lever_door_status(lever_id, true);
    object->tile_id = lever_activated_tile_id;

    // All other levers in the same room are deactivated
    const Lever* all_levers = (Lever*) levers;
    for (size_t i = 0; i < levers_count; i++)
    {
        const Lever* other_lever = &all_levers[i];
        if (other_lever->room_id == object->room_id &&
            other_lever != lever)
        {
            object_id_t object_id = get_object_at(object->room_id, other_lever->position);
            drop_object(object_id);
            change_lever_door_status(other_lever->lever_id, false);
            change_object_tile_id(object_id, lever_deactivated_tile_id);
        }
    }

    display_objects(object->room_id);
}
