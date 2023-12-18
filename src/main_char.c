#include "main_char.h"

#include "../generated/spawner_data.h"
#include "../generated/texts_data.h"
#include "characters.h"
#include "keys.h"
#include "print.h"
#include "room.h"
#include "text_zone.h"
#include "tiles.h"

unsigned char main_char_position = 0;
unsigned char main_char_old_position = 0;

character_stats_t initial_main_char_stats = {10,// hp
                                             10,// max_hp
                                             {
                                                     5, // attack
                                                     10,// defense
                                                     2, // damage
                                                     0  // resistance
                                             }};

battle_stats_t stats_weapon_1 = {10, 10, 3, 2};
battle_stats_t stats_weapon_2 = {20, 10, 10, 3};

character_stats_t current_char_stats = {0, 0, {0, 0, 0}};

void initialize_main_char()
{
    current_char_stats = initial_main_char_stats;
}

bool is_main_char_dead()
{
    return current_char_stats.hp == 0;
}

void main_char_change_weapon(int weapon_id)
{
    if (weapon_id == 1)
    {
        current_char_stats.battle_stats = stats_weapon_1;
        add_to_text_zone("Vous prenez l'ep@e");
    }
    else if (weapon_id == 2)
    {
        current_char_stats.battle_stats = stats_weapon_2;
        add_to_text_zone("");
        add_to_text_zone("Vous prenez le marteau");
        add_to_text_zone("Il semble pouvoir casser");
        add_to_text_zone("des murs !");
    }
    else
    {
        current_char_stats.battle_stats = initial_main_char_stats.battle_stats;
    }
}

bool is_main_char_at(unsigned char position)
{
    return main_char_position == position;
}

void main_char_teleport(unsigned char position, bool forget_old_position)
{
    if (forget_old_position)
    {
        main_char_cancel_last_move();
    }
    main_char_position = position;
}

EMoveDirection get_move_direction(unsigned char key)
{
    switch (key)
    {
        case KEY_RIGHT:
            return EMoveDirection_Right;
        case KEY_DOWN:
            return EMoveDirection_Down;
        case KEY_LEFT:
            return EMoveDirection_Left;
        case KEY_UP:
            return EMoveDirection_Up;
        default:
            return EMoveDirection_None;
    }
}

void main_char_move(unsigned char key,
                    bool (*can_move_to)(unsigned char),
                    void (*commit_move)(unsigned char))
{
    unsigned char new_position;
    new_position = main_char_position;
    main_char_old_position = main_char_position;

    EMoveDirection direction = get_move_direction(key);
    new_position = move_position_with_direction(new_position, direction);

    if (can_move_to(new_position))
    {
        main_char_position = new_position;
        commit_move(new_position);
    }
}

void main_char_display(unsigned char (*get_tile)(unsigned char) )
{
    const unsigned char previous_tile = get_tile(main_char_old_position);

    char x;
    char y;
    position_to_x_y(main_char_old_position, &x, &y);
    display_tile(previous_tile, x * 2, y * 2);

    position_to_x_y(main_char_position, &x, &y);
    display_tile(E_SPAWN_MC, x * 2, y * 2);
}

void main_char_cancel_last_move()
{
    main_char_position = main_char_old_position;
}

unsigned char get_main_char_position()
{
    return main_char_position;
}

void main_char_display_stats()
{
    const char * const hit_points = all_texts[TEXT_HP];
    print_encoded_str_at(21, 2, hit_points);

    goto_xy(25, 2);
    print_number(current_char_stats.hp);

    const char * const separator = all_texts[TEXT_SEPARATOR];
    print_encoded_str_at(27, 3, separator);
    print_number(current_char_stats.max_hp);
}

const battle_stats_t* get_main_char_stats()
{
    return &current_char_stats.battle_stats;
}

void main_char_apply_damage(unsigned char hp_change)
{
    if (hp_change == 0)
    {
        return;
    }

    add_to_text_zone("Vous etes touch@");
    if (hp_change >= current_char_stats.hp)
    {
        current_char_stats.hp = 0;
    }
    else
    {
        current_char_stats.hp -= hp_change;
    }
    main_char_display_stats();
}

void main_char_heal(unsigned char hp_change)
{
    if (hp_change + current_char_stats.hp > current_char_stats.max_hp)
    {
        current_char_stats.hp = current_char_stats.max_hp;
    }
    else
    {
        current_char_stats.hp += hp_change;
    }
    main_char_display_stats();
}
