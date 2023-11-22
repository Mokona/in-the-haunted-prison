#ifndef JEU_LYNX_CHARACTERS_H
#define JEU_LYNX_CHARACTERS_H

typedef struct
{
    unsigned char force;
    unsigned char defense;
    unsigned char damage;
    unsigned char resistance;
} battle_stats_t;

typedef struct
{
    unsigned char hp;
    unsigned char max_hp;

    battle_stats_t battle_stats;
} character_stats_t;

#endif
