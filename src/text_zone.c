#include "text_zone.h"
#include "../generated/font_data.h"
#include "print.h"

#include <malloc.h>
#include <stdbool.h>
#include <string.h>

#define TEXT_ZONE_LENGTH 200
char* text_zone_buffer;
char* end_of_zone;// Points on the first free byte of the buffer

#define FIRST_ZONE_LINE 22
#define LAST_ZONE_LINE 28

bool need_update_text_zone = false;

void initialize_text_zone()
{
    text_zone_buffer = (char*) malloc(TEXT_ZONE_LENGTH);
    end_of_zone = text_zone_buffer;

    text_zone_buffer[0] = 0;
}

void clear_line(char line) __z88dk_fastcall
{
    goto_xy(0, line);
    for (char col = 0; col < 32; ++col)
    {
        print_str(" ");
    }
    goto_xy(0, line);
}

void display_initial_text_zone()
{
    need_update_text_zone = true;
    end_of_zone = text_zone_buffer;
}

void add_to_text_zone(const char* text_to_add) __z88dk_fastcall
{
    size_t size_of_new_text = strlen(text_to_add) + 1;

    // Check if we need to space
    while ((end_of_zone + size_of_new_text) > (text_zone_buffer + TEXT_ZONE_LENGTH))
    {
        // Remove the first string from the buffer
        unsigned char len = strlen(text_zone_buffer) + 1;
        memmove(text_zone_buffer, text_zone_buffer + len, TEXT_ZONE_LENGTH - len);
        end_of_zone -= len;
    }

    strcpy(end_of_zone, text_to_add);
    for (char* current = end_of_zone; current < end_of_zone + size_of_new_text; ++current)
    {
        if (*current == glyph_count - 1)
        {
            *current = 0;
        }
    }
    end_of_zone += size_of_new_text;

    need_update_text_zone = true;
}

void update_text_zone()
{
    if (!need_update_text_zone)
    {
        return;
    }

    need_update_text_zone = false;

    // Display the lines from the end of the buffer (end_of_zone) back to start, starting with the last line
    char line = LAST_ZONE_LINE - 1;
    char* current = end_of_zone - 2;
    while (current >= text_zone_buffer && line > FIRST_ZONE_LINE)
    {
        while ((current > text_zone_buffer) && (*current != 0))
        {
            --current;
        }
        if (*current == 0)
        {
            ++current;
        }

        clear_line(line);
        print_encoded_str(current);

        current -= 2;
        --line;
    }
    // Clear the remaining lines
    while (line > FIRST_ZONE_LINE)
    {
        clear_line(line);
        --line;
    }
}
