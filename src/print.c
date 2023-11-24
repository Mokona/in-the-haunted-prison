#include "print.h"

#include "vector.h"

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
