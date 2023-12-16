#ifndef JEU_LYNX_PRINT_H
#define JEU_LYNX_PRINT_H

void goto_xy(char x, char y);
void print_str(const char* str);
void print_encoded_str(const char* str);
char print_encoded_str_at(char column, char line, const char* text);// Returns the line after the last printed line
void print_number(int number);
void put_char(char c);
char convert_char(char c);

#endif
