#include "vdp.h"
#include <graphics.h>
#include <stdio.h>

int main()
{
    cursor_enable(0);

    unsigned int x,y;
    x = getmaxx();
    y = getmaxy();
    printf("Screen size: %d x %d\n", x, y);

    while (1)
        ;
    return 0;
}
