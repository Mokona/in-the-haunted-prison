#include "mobs.h"

#include "../generated/spawner_data.h"
#include "../generated/texts_data.h"
#include "inventory.h"
#include "room.h"
#include "spawners.h"
#include "text_zone.h"
#include "tiles.h"
#include "utils.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    EMoveType_Static = 0x00,
    EMoveType_Random = 0x01,
    EMoveType_Follow = 0x02,
    EMoveType_Slow_Follow = 0x03,
    EMoveType_Escape = 0x04,
} EMoveType;

typedef struct {
    unsigned char spawner_id;
    unsigned char initial_health;
    unsigned char damage;
    unsigned char defense;
    unsigned char move_type;// Because enum is on 2 bytes
} Mob;

Mob mob_templates[] = {
        {E_SPAWN_BAT, 3, 1, 0, EMoveType_Random},
        {E_SPAWN_RAT, 5, 2, 0, EMoveType_Escape},
        {E_SPAWN_SKELETON, 6, 3, 3, EMoveType_Follow},
        {E_SPAWN_ZOMBIE, 10, 5, 5, EMoveType_Slow_Follow},
        {E_SPAWN_FANTOM, 20, 10, 30, EMoveType_Static},
        {E_SPAWN_WALL, 30, 0, 15, EMoveType_Static},
};

typedef struct {
    const Mob* mob;
    unsigned char position;
    unsigned char old_position;
    unsigned char room_id;
    unsigned char move_type;
    unsigned char force_display;// Used to avoid WALL blinking mainly
    character_stats_t battle_stats;
} ActiveMob;

// List of active mobs
#define MAX_ACTIVE_MOBS 10
ActiveMob active_mobs[MAX_ACTIVE_MOBS];
size_t active_mobs_count = 0;
bool can_spawn_mob = true;

void initialize_mobs()
{
    memset(active_mobs, 0, sizeof(active_mobs));
    active_mobs_count = 0;
    can_spawn_mob = true;
}

bool is_mob_alive(const ActiveMob* active_mob)
{
    return active_mob->battle_stats.hp > 0;
}

bool has_active_mob_in_room(unsigned char room_id)
{
    for (size_t i = 0; i < active_mobs_count; i++)
    {
        if (active_mobs[i].room_id == room_id)
        {
            return true;
        }
    }

    return false;
}

void remove_active_mob_at_index(size_t index)
{
    memcpy(&active_mobs[index],
           &active_mobs[index + 1],
           sizeof(ActiveMob) * (MAX_ACTIVE_MOBS - index - 1));

    active_mobs_count--;
}

void remove_oldest_active_mob()
{
    if (active_mobs_count == 0)
    {
        return;
    }

    // All the mobs from the same rooms must be removed
    // with the oldest one, or it causes bugs (with ghosts
    // for example, which are then not blocked the way)

    const unsigned char room_id = active_mobs[0].room_id;
    while (active_mobs_count > 0 && active_mobs[0].room_id == room_id)
    {
        remove_active_mob_at_index(0);
    }
}

void reset_old_mobs_position_for_room(unsigned char room_id)
{
    for (size_t i = 0; i < active_mobs_count; i++)
    {
        ActiveMob* active_mob = &active_mobs[i];
        if (active_mob->room_id == room_id)
        {
            active_mob->old_position = active_mob->position;
            active_mob->force_display = true;// To avoid WALL blinking
        }
    }
}

const Mob* get_mob_template_from_id(unsigned char id)
{
    // Returns the mob template from its spawn ID
    for (size_t i = 0; i < sizeof(mob_templates) / sizeof(Mob); i++)
    {
        if (mob_templates[i].spawner_id == id)
        {
            return &mob_templates[i];
        }
    }
}

void spawn_mob(room_id_t room_id, unsigned char position, unsigned char mob_id)
{
    if (mob_id == E_SPAWN_MC || !can_spawn_mob)
    {
        // Spawner for main character is special
        // And if the game is finished, there's no spawn anymore
        return;
    }

    if (active_mobs_count >= MAX_ACTIVE_MOBS)
    {
        remove_oldest_active_mob();
    }

    ActiveMob* spawning_mob = &active_mobs[active_mobs_count];

    spawning_mob->mob = get_mob_template_from_id(mob_id);
    spawning_mob->position = position;
    spawning_mob->old_position = position;
    spawning_mob->room_id = room_id;
    spawning_mob->move_type = spawning_mob->mob->move_type;
    spawning_mob->force_display = true;

    const Mob* mob = spawning_mob->mob;
    spawning_mob->battle_stats.hp = spawning_mob->battle_stats.max_hp = mob->initial_health;

    battle_stats_t* stats = &(spawning_mob->battle_stats.battle_stats);
    stats->force = 20;// Mob always hits
    stats->defense = mob->defense;
    stats->damage = mob->damage;
    stats->resistance = 0;
    active_mobs_count++;
}

void activate_mob_for_room(unsigned char room_id)
{
    if (has_active_mob_in_room(room_id))
    {
        // If at least one mob is still present in the room, they are
        // not reset. But we need to reset their old position as if
        // they hadn't moved.
        reset_old_mobs_position_for_room(room_id);
        return;
    }

    // Else, mobs are spawned
    iterate_spawners_for_room(room_id, spawn_mob);
}

bool must_be_displayed(const ActiveMob* mob)
{
    return mob->position != mob->old_position ||
           mob->force_display;
}

void display_mobs(unsigned char room_id,
                  unsigned char (*get_tile)(unsigned char))
{
    char x;
    char y;

    for (size_t i = 0; i < active_mobs_count; i++)
    {
        ActiveMob* active_mob = &active_mobs[i];

        if (active_mob->room_id == room_id && must_be_displayed(active_mob))
        {
            const unsigned char previous_tile = get_tile(active_mob->old_position);

            position_to_x_y(active_mob->old_position, &x, &y);
            display_tile(previous_tile, (char) (x * 2), (char) (y * 2));

            if (is_mob_alive(active_mob))
            {
                position_to_x_y(active_mob->position, &x, &y);
                display_tile(active_mob->mob->spawner_id, (char) (x * 2), (char) (y * 2));
                active_mob->force_display = false;
            }
        }
    }
}

EMoveDirection get_direction_to_follow(unsigned char position, unsigned char attractor_position)
{
    char x;
    char y;
    position_to_x_y(position, &x, &y);

    char attractor_x;
    char attractor_y;
    position_to_x_y(attractor_position, &attractor_x, &attractor_y);

    signed char dx = (signed char) (attractor_x - x);
    signed char dy = (signed char) (attractor_y - y);

    if (dx == 0 && dy == 0)
    {
        return EMoveDirection_None;
    }

    if (abs(dx) > abs(dy))
    {
        return (dx > 0) ? EMoveDirection_Right : EMoveDirection_Left;
    }
    else
    {
        return (dy > 0) ? EMoveDirection_Down : EMoveDirection_Up;
    }
}

EMoveDirection opposite_direction(EMoveDirection move)
{
    switch (move)
    {
        case EMoveDirection_Up:
            return EMoveDirection_Down;
        case EMoveDirection_Down:
            return EMoveDirection_Up;
        case EMoveDirection_Left:
            return EMoveDirection_Right;
        case EMoveDirection_Right:
            return EMoveDirection_Left;
        default:
            return EMoveDirection_None;
    }
}

void move_one_mob(ActiveMob* active_mob, bool (*can_move_to)(unsigned char), unsigned char attractor_position)
{
    unsigned char new_position = active_mob->position;
    active_mob->old_position = active_mob->position;

    switch (active_mob->move_type)
    {
        case EMoveType_Random: {
            EMoveDirection move = (EMoveDirection) (rand() % 4);// NOLINT(*-msc50-cpp)
            new_position = move_position_with_direction(new_position, move);
        }
        break;
        case EMoveType_Slow_Follow: {
            if (rand() % 2 == 0)// NOLINT(*-msc50-cpp)
            {
                break;
            }
        }
            // no break
        case EMoveType_Follow: {
            EMoveDirection move = get_direction_to_follow(new_position, attractor_position);
            new_position = move_position_with_direction(new_position, move);
        }
        break;
        case EMoveType_Escape: {
            EMoveDirection move = get_direction_to_follow(new_position, attractor_position);
            move = opposite_direction(move);
            new_position = move_position_with_direction(new_position, move);
        }
        break;
        case EMoveType_Static:
            return;
        default:
            break;
    }

    if (can_move_to(new_position))
    {
        active_mob->position = new_position;
    }
}

void mobs_move(room_id_t room_id,
               bool (*can_move_to)(unsigned char),
               unsigned char attractor_position)
{
    for (size_t i = 0; i < active_mobs_count; i++)
    {
        ActiveMob* active_mob = &active_mobs[i];

        if (active_mob->room_id == room_id)
        // We can move dead mobs, are they are static. It takes a bit less code space && is_mob_alive(active_mob))
        {
            move_one_mob(active_mob,
                         can_move_to,
                         attractor_position);
        }
    }
}

void mobs_clean()
{
    size_t i = active_mobs_count;

    while (i > 0)
    {
        i -= 1;
        ActiveMob* active_mob = &active_mobs[i];

        if (!is_mob_alive(active_mob))
        {
            remove_active_mob_at_index(i);
        }
    }
}

mob_id_t get_mob_at(room_id_t room_id, unsigned char position)
{
    for (size_t i = 0; i < active_mobs_count; i++)
    {
        ActiveMob* active_mob = &active_mobs[i];

        if (active_mob->room_id == room_id &&
            active_mob->position == position)
        {
            return i;
        }
    }

    return NO_MOB;
}

void kill_mob(mob_id_t mob_id)
{
    ActiveMob* active_mob = &active_mobs[mob_id];
    active_mob->battle_stats.hp = 0;
    active_mob->move_type = EMoveType_Static;
    active_mob->old_position = active_mob->position;
    active_mob->force_display = true;
}

const battle_stats_t* get_mob_stats(mob_id_t id)
{
    const ActiveMob* active_mob = &active_mobs[id];

    return &(active_mob->battle_stats.battle_stats);
}

void mob_apply_damage(mob_id_t id, unsigned char hp_change)
{
    if (hp_change == 0)
    {
        return;
    }

    ActiveMob* active_mob = &active_mobs[id];

    bool is_wall = active_mob->mob->spawner_id == E_SPAWN_WALL;
    if (!is_wall)
    {
        const char * const hit = all_texts[TEXT_MONSTER_HIT];
        add_to_text_zone(hit);
    }
    else
    {
        const char * const wall = all_texts[TEXT_WALL];
        add_to_text_zone(wall);
    }

    if (hp_change >= active_mob->battle_stats.hp)
    {
        kill_mob(id);

        unsigned char spawner_id = active_mob->mob->spawner_id;
        if (!is_in_inventory(spawner_id) && !is_wall)
        {
            const char * const trophy = all_texts[TEXT_TROPHY];
            add_to_text_zone(trophy);
            add_to_inventory(spawner_id);
        }
    }
    else if (hp_change > 0)
    {
        active_mob->battle_stats.hp -= hp_change;
    }
}

void clear_game()
{
    // kill all mobs
    for (size_t i = 0; i < active_mobs_count; i++)
    {
        kill_mob(i);
    }
    can_spawn_mob = false;
}
