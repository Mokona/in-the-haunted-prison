#include "screen.h"

#if defined(__AGON__)

#include <graphics.h>
void clear_screen()
{
    clg();
}

#endif//__AGON__

#if defined(__LYNX__)

extern void lynx_clear_screen();
void clear_screen()
{
    lynx_clear_screen();
}

#endif//__LYNX__
