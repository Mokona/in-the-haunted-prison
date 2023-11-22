#include "battle.h"

#include <stdlib.h>

battle_result_t result;

unsigned char resolve_attack(const battle_stats_t* attacker, const battle_stats_t* defender)
{
    char attack_roll = (rand() & 7) + attacker->force;
    char defense_roll = (rand() & 7) + defender->defense;

    if (attack_roll > defense_roll)
    {
        signed char damage = (signed char) attacker->damage - (signed char) defender->resistance;
        if (damage > 0)
        {
            return (unsigned char) damage;
        }
    }

    return 0;
}

const battle_result_t* battle(const battle_stats_t* attacker, const battle_stats_t* defender)
{
    result.lost_hp_1 = resolve_attack(attacker, defender);
    result.lost_hp_2 = resolve_attack(defender, attacker);

    return &result;
}