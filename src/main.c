#include "agon/vdp.h"
#include <graphics.h>
#include <stdio.h>

#include "clynx/check_96k.h"
#include "font.h"
#include "loop_title.h"
#include "memory.h"
#include "mobs.h"
#include "room.h"
#include "text_zone.h"

#include <stdbool.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main()
{
#if defined(__AGON__)
    cursor_enable(0);
#endif

#if defined(__LYNX__)
    check_96k();
#endif

    init_malloc();
    initialize_font();
    initialize_rooms();
    initialize_mob_graphs();
    initialize_text_zone();

    start_title_loop();

    while (true)
    {
        loop_title();
    }
}
#pragma clang diagnostic pop
