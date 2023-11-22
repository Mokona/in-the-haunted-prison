#include "loop_retry.h"

#include "print.h"
#include "loop_main.h"// ... TODO: remove this include

#include <input.h>
#include <sys/types.h>

extern void clear_screen();

extern uint previous_key;
extern uint key;

const char* retry_text[] = {
        "Vous avez fui la prison",
        "hant@e, mais vous n'avez",
        "pas trouv@ le tr@sor.",
        "",
        "Allez-vous r@essayer ?",
        NULL};

const char* dead_text[] = {
        "Vous @tes mort...",
        "",
        "Mais ce n'est pas cela",
        "qui va vous arr@ter !",
        "",
        "Allez-vous r@essayer ?",
        NULL};

void start_retry_loop(int reason)
{
    exit_reason_t exit_reason = (exit_reason_t) reason;

    clear_screen();

    const char * const * text = exit_reason == exit_reason_dead ? dead_text : retry_text;

    for (int i = 0; text[i] != NULL; i++)
    {
        goto_xy(2, 11 + i);
        print_str(text[i]);
    }

    goto_xy(7, 25);
    print_str("ESPACE pour r@essayer...");
    previous_key = in_Inkey();
}

int loop_retry()
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
