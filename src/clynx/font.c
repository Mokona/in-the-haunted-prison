#include "../font.h"

#include "../../generated/font_data.h"

#include "../../generated/font_data.h"

extern void copy_8x8_mono_to_vram(const char* char_data, unsigned short destination);

static unsigned short current_location;

void initialize_font()
{
    current_location = 0;
}

void put_char(char c)
{
    char* char_address = (char*) &font_data;
    char_address += c * 8;

    copy_8x8_mono_to_vram(char_address, current_location);

    current_location += 1;
}

void goto_xy(char x, char y)
{
    current_location = y;
    current_location *= 32;
    current_location *= 8;
    current_location += x;
}
