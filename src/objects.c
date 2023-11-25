#include "objects.h"

#include "../generated/objects_data.h"
#include "../generated/spawner_data.h"
#include "doors.h"
#include "inventory.h"
#include "levers.h"
#include "main_char.h"
#include "mobs.h"
#include "room.h"
#include "text_zone.h"
#include "tiles.h"

#include <stdbool.h>

void initialize_objects()
{
    Object* all_objects = (Object*) objects;
    // Drop all objects on ground
    for (size_t i = 0; i < objects_count; i++)
    {
        Object* object = &all_objects[i];
        object->picked = false;
    }
}

void display_objects(room_id_t room_id)
{
    const Object* all_objects = (Object*) objects;

    for (size_t i = 0; i < objects_count; i++)
    {
        const Object* object = &all_objects[i];
        if (object->room_id == room_id && (!object->picked || object->properties & E_LEVER))
        {
            char x;
            char y;
            position_to_x_y(object->position, &x, &y);
            display_tile(object->tile_id, x * 2, y * 2);
        }
    }
}

object_id_t get_object_at(room_id_t room_id, unsigned char position)
{
    const Object* all_objects = (Object*) objects;

    for (size_t i = 0; i < objects_count; i++)
    {
        const Object* object = &all_objects[i];
        if (object->room_id == room_id && (!object->picked || object->properties & E_LEVER))
        {
            if (object->position == position)
            {
                return i;
            }
        }
    }

    return NO_OBJECT;
}

bool is_pickable(const Object* object)
{
    return object->properties & E_PICKABLE && !object->picked;
}

const unsigned char mob_spawner_tile_ids[] = {
        E_SPAWN_ZOMBIE,
        // E_SPAWN_FANTOM, -> invincible
        E_SPAWN_SKELETON,
        E_SPAWN_BAT,
        E_SPAWN_RAT};

bool act_on_cauldron()
{
    // Check that every mob spawner tile id is in the inventory
    bool has_all_items = true;

    for (size_t i = 0; i < sizeof(mob_spawner_tile_ids); i++)
    {
        unsigned char tile_id = mob_spawner_tile_ids[i];
        has_all_items &= is_in_inventory(tile_id);
    }

    if (has_all_items)
    {
        clear_game();
        add_to_text_zone("");
        add_to_text_zone("Le sort fait effet");
        add_to_text_zone("et dans un grand souffle,");
        add_to_text_zone("les monstres disparaissent.");
        return true;
    }
    else
    {
        add_to_text_zone("");
        add_to_text_zone("Rien ne se passe.");
        add_to_text_zone("Il manque probablement");
        add_to_text_zone("des ingr@dients.");
        return false;
    }
}

void act_on_object(object_id_t object_id)
{
    Object* all_objects = (Object*) objects;
    Object* object = &all_objects[object_id];
    if (is_pickable(object))
    {
        object->picked = true;

        if (object->properties & E_KEY)
        {
            open_door_with_key(object->tile_id);
        }
        else if (object->properties & E_HP)
        {
            main_char_heal(5);
        }
        else if (object->properties & E_WEAPON_1 || object->properties & E_WEAPON_2)
        {
            main_char_change_weapon(object->properties & E_WEAPON_1 ? 1 : 2);
        }
        add_to_inventory(object->tile_id);
    }
    else if (object->properties & E_LEVER)
    {
        object->picked = true;
        act_on_lever(object);
    }
    else if (object->properties & E_CAULDRON)
    {
        if (act_on_cauldron())
        {
            object->picked = true;
        }
    }
}

void drop_object(object_id_t object_id)
{
    // Naming is misleading, as it's only used for levers. We don't drop objects in this game
    Object* all_objects = (Object*) objects;
    Object* object = &all_objects[object_id];
    object->picked = false;
}

unsigned char get_object_tile_id(object_id_t object_id)
{
    const Object* all_objects = (Object*) objects;
    const Object* object = &all_objects[object_id];
    return object->tile_id;
}

void change_object_tile_id(object_id_t id, unsigned char tile_id)
{
    Object* all_objects = (Object*) objects;
    all_objects[id].tile_id = tile_id;
}
