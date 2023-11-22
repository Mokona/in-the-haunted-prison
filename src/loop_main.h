#ifndef JEU_LYNX_LOOP_MAIN_H
#define JEU_LYNX_LOOP_MAIN_H

typedef enum
{
    exit_reason_continue,
    exit_reason_victory,
    exit_reason_dead,
    exit_reason_escape,
} exit_reason_t;

void start_main_loop();
exit_reason_t main_game_loop();

#endif
