#include "loop_title.h"

#include "loop_main.h"
#include "loop_retry.h"
#include "loop_victory.h"
#include "print.h"

#include <input.h>
#include <stdbool.h>

extern void clear_screen();

extern uint previous_key;
extern uint key;

const char* title_text[] = {
        "Devant la porte de cette",
        "prison hant@e vous h@sitez",
        "encore un instant.",
        "",
        "Aurez-vous le courage d'y",
        "chercher le fameux tr@sor",
        "dont cet @trange vieillard",
        "vous a parl@ ?",
        NULL};

void start_title_loop()
{
    clear_screen();

    // Display the title screen
    goto_xy(5, 3);
    print_str("DANS LA PRISON HANTEE");

    goto_xy(9, 6);
    print_str("Par S.Glaize");
    goto_xy(2, 7);
    print_str("Tuiles par Kenney.nl");

    // Display the text
    for (int i = 0; title_text[i] != NULL; i++)
    {
        goto_xy(2, 11 + i);
        print_str(title_text[i]);
    }

    goto_xy(10, 25);
    print_str("ESPACE pour entrer...");
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
