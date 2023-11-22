#include "spawners.h"

#include "../generated/spawner_data.h"

typedef struct {
    unsigned char room_id;
    unsigned char position;
    unsigned char type;
} Spawner;

unsigned char get_main_character_spawn_position(unsigned char* room_id)
{
    const Spawner* spawner_array = (const Spawner*) spawners;

    // Search for the main character spawner
    for (size_t i = 0; i < spawners_count; i++)
    {
        const Spawner* spawner = &spawner_array[i];

        if (spawner->type == E_SPAWN_MC)
        {
            *room_id = spawner->room_id;
            return spawner->position;
        }
    }
}

void iterate_spawners_for_room(room_id_t room_id, spawner_callback_t callback)
{
    const Spawner* spawner_array = (const Spawner*) spawners;

    for (size_t i = 0; i < spawners_count; i++)
    {
        const Spawner* spawner = &spawner_array[i];

        if (spawner->room_id == room_id)
        {
            callback(room_id,
                     spawner->position,
                     spawner->type);
        }
    }
}
