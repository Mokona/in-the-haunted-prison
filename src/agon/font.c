#include "../font.h"

#include "../../generated/font_data.h"

#include "vdp.h"

extern void my_agon_putc(char c) __z88dk_fastcall;

static unsigned short current_location_x;
static unsigned short current_location_y;

void initialize_font()
{
    for (size_t i = 0; i < glyph_count; i++)
    {
        const unsigned char* glyph = &font_data[i * 8];
        bitmap_select(i);
        bitmap_col_load(8, 8);
        for (size_t line = 0; line < 8; line++)
        {
            unsigned char column_colors = glyph[line];

            for(size_t column = 0; column < 8; column++)
            {
                unsigned char color = column_colors & 0x80 ? 0xFF : 0x00;
                column_colors <<= 1;

                my_agon_putc(color);
                my_agon_putc(color);
                my_agon_putc(color);
                my_agon_putc(0xFF);
            }
        }
    }
}

void put_char(char c)
{
    bitmap_select(c);
    bitmap_draw(current_location_x, current_location_y);
    current_location_x += 8;
}

void goto_xy(char x, char y)
{
    current_location_x = x * 8;
    current_location_y = y * 8;
}
