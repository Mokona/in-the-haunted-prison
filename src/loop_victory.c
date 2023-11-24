#include "loop_victory.h"

#include "print.h"
#include "inventory.h"

#include <input.h>
#include <sys/types.h>

extern void clear_screen();

extern uint previous_key;
extern uint key;

const char* victory_text[] = {
        "Bravo !",
        "",
        "Vous avez trouv@ le tr@sor !",
        "",
        "Vous avez @limin@ la",
        "menace de la prison hant@e.",
        NULL};

const char* secret_found_text[] = {
        "Vous avez aussi trouv@ le",
        "tr@sor secret !",
        "",
        "",
        "",
        "Merci d'avoir jou@ !",
        NULL};

const char* secret_not_found_text[] = {
        "Mais vous n'avez pas trouv@",
        "le tr@sor secret...",
        NULL};

void start_victory_loop()
{
    clear_screen();

    int line = 3;

    for (int i = 0; victory_text[i] != NULL; i++)
    {
        goto_xy(2, line);
        print_str(victory_text[i]);
        line++;
    }

    line+=4;

    bool found_secret = has_secret_been_found();
    const char * const * secret_text = found_secret ? secret_found_text : secret_not_found_text;

    for (int i = 0; secret_text[i] != NULL; i++)
    {
        goto_xy(2, line);
        print_str(secret_text[i]);
        line++;
    }

    goto_xy(7, 25);
    print_str("ESPACE pour rejouer...");
    previous_key = in_Inkey();
}

int loop_victory()
{
    key = in_Inkey();
    if (key != 0 && key != previous_key)
    {
        if (key == 32)
        {
            return 1;
        }
    }
    return 0;
}