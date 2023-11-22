#ifndef JEU_LYNX_BATTLE_H
#define JEU_LYNX_BATTLE_H

#include "characters.h"

typedef struct
{
    unsigned char lost_hp_1;
    unsigned char lost_hp_2;
} battle_result_t;

const battle_result_t* battle(const battle_stats_t* attacker, const battle_stats_t* defender);

#endif
