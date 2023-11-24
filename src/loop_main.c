#include "loop_main.h"

#include "battle.h"
#include "doors.h"
#include "inventory.h"
#include "levers.h"
#include "main_char.h"
#include "mobs.h"
#include "objects.h"
#include "room.h"
#include "screen.h"
#include "spawners.h"
#include "teleport.h"
#include "text_zone.h"
#include "tiles.h"

#include <input.h>

uint previous_key = 0;
uint key = 0;
exit_reason_t exit_reason = exit_reason_continue;

bool can_mc_move_to(unsigned char position)
{
    unsigned char property = get_room_tile_property_at(position);

    // No need to check on property_is_door, if there's no door,
    // the function will return false
    if (/* property_is_door(property) &&*/ is_door_locked(position))
    {
        return false;
    }

    return !property_is_blocking(property);
}

bool can_mob_move_to(unsigned char position)
{
    unsigned char property = get_room_tile_property_at(position);
    unsigned char mob_id = get_mob_at(get_current_room_id(), position);
    return !property_is_blocking(property) && !is_main_char_at(position) && mob_id == NO_MOB;
}

const unsigned char* get_tile(unsigned char position)
{
    object_id_t id = get_object_at(get_current_room_id(), position);
    if (id != NO_OBJECT)
    {
        unsigned char tile_id = get_object_tile_id(id);
        return get_tile_graph(tile_id);
    }
    return get_room_tile_at(position);
}

void change_and_display_room(unsigned char room_id)
{
    change_room(room_id);
    activate_mob_for_room(get_current_room_id());
    display_room();
    display_mobs(get_current_room_id(), get_tile);
}

void commit_move(unsigned char position)
{
    room_id_t current_room_id = get_current_room_id();
    mob_id_t touched_mob_id = get_mob_at(current_room_id, position);
    if (touched_mob_id != NO_MOB)
    {
        // Get main char combat stats
        // Get mob combat stats
        const battle_stats_t* main_char_stats = get_main_char_stats();
        const battle_stats_t* mob_stats = get_mob_stats(touched_mob_id);

        // Initiate battle simulation
        const battle_result_t* battle_result = battle(main_char_stats, mob_stats);

        const unsigned char lost_hp_1 = battle_result->lost_hp_1;
        const unsigned char lost_hp_2 = battle_result->lost_hp_2;
        if (lost_hp_1 + lost_hp_2 > 0)
        {
            add_to_text_zone("");
        }

        main_char_apply_damage(lost_hp_2);
        mob_apply_damage(touched_mob_id, lost_hp_1);

        // kill_mob(touched_mob_id);
        main_char_cancel_last_move();
    }
    else
    {
        // Movement between rooms
        const Teleport* teleport = get_teleport_at(current_room_id, position);

        if (teleport != NULL)
        {
            unsigned char destination_room = teleport->destination_room_id;
            if (destination_room == 255)
            {
                // Escape condition
                exit_reason = exit_reason_escape;
            }
            else if (destination_room == 254)
            {
                // Victory condition
                exit_reason = exit_reason_victory;
            }
            else
            {
                change_and_display_room(destination_room);
                main_char_teleport(teleport->destination_position, true);
            }
        }
        else
        {
            unsigned char destination_room = get_neighbour_room_id(current_room_id, &position);

            if (destination_room != current_room_id)
            {
                change_and_display_room(destination_room);
                main_char_teleport(position, true);
            }
        }

        // Objects
        object_id_t object_id = get_object_at(current_room_id, position);
        if (object_id != NO_OBJECT)
        {
            act_on_object(object_id);
        }
    }

    main_char_display(get_tile);
}

void start_main_loop()
{
    clear_screen();

    initialize_main_char();
    initialize_doors();
    initialize_objects();
    initialize_levers();
    initialize_inventory();
    initialize_mobs();

    unsigned char mc_spawn_room_id = 0;
    unsigned char mc_spawn_position = get_main_character_spawn_position(&mc_spawn_room_id);

    change_and_display_room(mc_spawn_room_id);
    display_initial_text_zone();

    main_char_teleport(mc_spawn_position, false);
    main_char_display(get_tile);

    main_char_display_stats();

    previous_key = in_Inkey();
    exit_reason = exit_reason_continue;
}

exit_reason_t main_game_loop()
{
    key = in_Inkey();

    room_id_t current_room_id = get_current_room_id();
    if (key != 0 && key != previous_key)
    {
        main_char_move(key, can_mc_move_to, commit_move);

        room_id_t new_room_id = get_current_room_id();
        if (new_room_id == current_room_id)
        {
            // Move mobs only if we didn't change room
            const unsigned char attractor_position = get_main_char_position();
            mobs_move(get_current_room_id(),
                      can_mob_move_to,
                      attractor_position);
        }

        display_mobs(new_room_id, get_tile);
        mobs_clean();

        if (is_main_char_dead())
        {
            exit_reason = exit_reason_dead;
        }
    }
    update_text_zone();

    previous_key = key;

    return exit_reason;
}
