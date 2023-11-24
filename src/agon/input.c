#include <stdint.h>

extern void getk();

uint __LIB__ in_Inkey(void)
{
    return getk();
}
