#include "loop_title.h"

#include "../generated/font_data.h"
#include "../generated/texts_data.h"
#include "loop_main.h"
#include "loop_retry.h"
#include "loop_victory.h"
#include "print.h"

#include <input.h>
#include <stdbool.h>

extern void clear_screen();

extern uint previous_key;
extern uint key;

#if LANGUAGE == 0
const char* title = "DANS LA PRISON HANTEE";
const char* author = "Par S.Glaize";
const char* graphics = "Tuiles par Kenney.nl";
const char* space_to_enter = "ESPACE pour entrer...";

#elif LANGUAGE == 1

const char* title = "IN THE HAUNTED PRISON";
const char* author = "By S.Glaize";
const char* graphics = "Tiles by Kenney.nl";
const char* space_to_enter = "SPACE to enter...";

#elif LANGUAGE == 2

const char* title = "EN LA SPUKITA PRIZONO";
const char* author = "De S.Glaize";
const char* graphics = "Bildoj de Kenney.nl";
const char* space_to_enter = "SPACO por eniri...";

#elif
#error "Language not defined"
#endif

void start_title_loop()
{
    clear_screen();

    // Display the title screen
    goto_xy(5, 3);
    print_str(title);

    goto_xy(9, 6);
    print_str(author);
    goto_xy(2, 7);
    print_str(graphics);

    const char * const text = all_texts[TEXT_ENTRY_TEXT];
    const char * text_pointer = text;
    char line = 11;
    char x = 2;
    while (*text_pointer != '\0')
    {
        if (*text_pointer == glyph_count -1)
        {
            line++;
            x = 2;
        }
        else
        {
            goto_xy(x, line);
            put_char(*text_pointer);
            x++;
        }
        text_pointer++;
    }

    goto_xy(10, 25);
    print_str(space_to_enter);
    previous_key = in_Inkey();
}

void launch_main_loop()
{
    volatile bool loop = true;

    start_main_loop();

    while (loop)
    {
        exit_reason_t reason = main_game_loop();
        if (reason == exit_reason_escape || reason == exit_reason_dead)
        {
            start_retry_loop(reason);
            while (!loop_retry())
            {
            }
            loop = false;
        }
        else if (reason == exit_reason_victory)
        {
            start_victory_loop();
            while (!loop_victory())
            {
            }
            loop = false;
        }
    }
}

void loop_title()
{
    key = in_Inkey();

    if (key != 0 && key != previous_key)
    {
        if (key == 32)
        {
            while (true)
            {
                launch_main_loop();
            }
        }
    }
}
