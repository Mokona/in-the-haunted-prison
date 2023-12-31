#include "print.h"
#include "../generated/font_data.h"

char convert_char(char c)
{
    if (c >= '?' && c <= 'Z')
    {
        return c - '?' + 18;
    }
    if (c >= 'a' && c <= 'z')
    {
        return c - 'a' + 14 + 32;
    }
    if (c == ' ')
    {
        return 1;
    }
    if (c >= '-' && c <= ':')
    {
        return c - '-' + 4;
    }
    if (c == '\'')
    {
        return 3;
    }
    if (c == '!')
    {
        return 2;
    }
    return 1;
}

void print_str(const char* str)
{
    while (*str != '\0')
    {
        put_char(convert_char(*str));
        str++;
    }
}

void print_encoded_str(const char* str)
{
    while (*str != '\0')
    {
        put_char(*str);
        str++;
    }
}

char print_encoded_str_at(char column, char line, const char* text)
{
    const char* text_pointer = text;
    goto_xy(column, line);
    while (*text_pointer != '\0')
    {
        if (*text_pointer == glyph_count - 1)
        {
            line++;
            goto_xy(column, line);
        }
        else
        {
            put_char(*text_pointer);
        }
        text_pointer++;
    }

    return line;
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
        buffer[i] = (number % 10) + 7;
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
