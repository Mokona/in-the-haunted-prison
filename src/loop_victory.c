#include "loop_victory.h"

#include "../generated/texts_data.h"
#include "inventory.h"
#include "print.h"

#include <input.h>

extern void clear_screen();

extern uint previous_key;
extern uint key;

void start_victory_loop()
{
    clear_screen();

    const char* const victory_text = all_texts[TEXT_WIN];
    const char* const secret_found_text = all_texts[TEXT_WIN_SECRET];
    const char* const secret_not_found_text = all_texts[TEXT_WIN_NO_SECRET];

    char line = print_encoded_str_at(2, 3, victory_text);

    bool found_secret = has_secret_been_found();
    const char* const secret_text = found_secret ? secret_found_text : secret_not_found_text;

    line += 4;
    print_encoded_str_at(2, line, secret_text);

    const char* const space_to_retry = all_texts[TEXT_SPACE_TO_REPLAY];
    print_encoded_str_at(7, 25, space_to_retry);
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
