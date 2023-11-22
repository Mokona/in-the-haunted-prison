#include "utils.h"

char bound_char(char value, char min, char max)
{
    if (value < min)
    {
        return min;
    }
    if (value >= max)
    {
        return max - 1;
    }
    return value;
}
