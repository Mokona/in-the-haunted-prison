#include "doors.h"
#include "room.h"

#include "../generated/doors_data.h"

// Doors properties
typedef struct Door {
    unsigned char room_id;
    char position;
    unsigned char key_lever_id : 6;
    unsigned char type : 1;// 0 -> key 1 -> lever
    unsigned char locked : 1;
} Door;

void initialize_doors()
{
    Door* all_doors = (Door*) doors;
    // Lock all doors
    for (size_t i = 0; i < doors_count; i++)
    {
        Door* door = &all_doors[i];
        door->locked = true;
    }
}

bool is_door_locked(unsigned char position)
{
    room_id_t current_room_id = get_current_room_id();

    const Door* all_doors = (Door*) doors;

    for (size_t i = 0; i < doors_count; i++)
    {
        const Door* door = &all_doors[i];
        if (door->room_id == current_room_id)
        {
            if (door->position == position)
            {
                return door->locked;
            }
        }
    }

    return false;
}

void change_door_or_lever_status(unsigned char id, bool open, bool act_on_lever)
{
    Door* all_doors = (Door*) doors;

    for (size_t i = 0; i < doors_count; i++)
    {
        Door* door = &all_doors[i];
        if (door->key_lever_id == id)
        {
            if (act_on_lever && door->type)
            {
                door->locked = !open;
            }
            else if (!act_on_lever && !door->type)
            {
                door->locked = false;
            }
        }
    }
}

void patch_level_with_doors(room_id_t id, unsigned char* data)
{
    // Check graph data and search for lever doors tiles
    // If found, patch the tile depending on the door status
    const Door* all_doors = (Door*) doors;

    for (size_t i = 0; i < doors_count; i++)
    {
        const Door* door = &all_doors[i];
        if (door->room_id == id)
        {
            unsigned char* tile = &data[door->position];
            if (door->type)// Lever door
            {
                if (door->locked)
                {
                    *tile = closed_lever_door_tile_id;
                }
                else
                {
                    *tile = opened_lever_door_tile_id;
                }
            }
        }
    }
}
