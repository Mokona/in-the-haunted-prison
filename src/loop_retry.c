#include "loop_retry.h"

#include "../generated/texts_data.h"
#include "loop_main.h"// ... TODO: remove this include
#include "print.h"

#include <input.h>
#include <sys/types.h>

extern void clear_screen();

extern uint previous_key;
extern uint key;

void start_retry_loop(int reason)
{
    exit_reason_t exit_reason = (exit_reason_t) reason;

    clear_screen();

    const char* const text = exit_reason == exit_reason_dead ? all_texts[TEXT_DEAD] : all_texts[TEXT_FLEE];
    print_encoded_str_at(2, 11, text);

    goto_xy(7, 25);
    print_encoded_str_at(7, 25, all_texts[TEXT_SPACE_TO_RETRY]);
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
