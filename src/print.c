#include "print.h"

#include "vector.h"

extern void copy_8x8_mono_to_vram(const char* char_data, unsigned short destination);

extern char* font_data;

static unsigned short current_location;

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

char convert_char(char c)
{
    if (c >= '?' && c <= 'Z')
    {
        return c - '?' + 17;
    }
    if (c >= 'a' && c <= 'z')
    {
        return c - 'a' + 14 + 31;
    }
    if (c == ' ')
    {
        return 0;
    }
    if (c >= '-' && c <= ':')
    {
        return c - '-' + 3;
    }
    if (c == '\'')
    {
        return 2;
    }
    if (c == '!')
    {
        return 1;
    }
    return 0;
}

void print_str(const char* str)
{
    while (*str != '\0')
    {
        put_char(convert_char(*str));
        str++;
    }
}

void print_number(int number)
{
    if (number == 0)
    {
        put_char(convert_char('0'));
        return;
    }

    char buffer[10];
    int i = 0;
    while (number > 0)
    {
        buffer[i] = (number % 10) + 6;
        number /= 10;
        i++;
    }
    i--;
    while (i >= 0)
    {
        put_char(buffer[i]);
        i--;
    }
}
