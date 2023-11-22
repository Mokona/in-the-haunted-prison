#ifndef JEU_LYNX_TEXT_ZONE_H
#define JEU_LYNX_TEXT_ZONE_H

void initialize_text_zone();
void display_initial_text_zone();

void add_to_text_zone(const char * text_to_add) __z88dk_fastcall;
void add_num_to_text_zone(int number) __z88dk_fastcall;
void clear_line(char line) __z88dk_fastcall;

void update_text_zone();

#endif
