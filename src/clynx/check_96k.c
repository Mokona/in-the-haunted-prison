#include "check_96k.h"
#include "../../generated/texts_data.h"
#include "../print.h"

extern void clear_screen();

void check_96k()
{
    unsigned char value = *(unsigned char*) 0xBFF8;
    *(unsigned char*) 0xBFF8 = 0xFF;
    if (*(unsigned char*) 0x9FF8 == 0xFF)
    {
        *(unsigned char*) 0xBFF8 = 0x00;
        if (*(unsigned char*) 0x9FF8 == 0x00)
        {
            // 48k Lynx
            // Clear screen and display error message then infinite loop
            clear_screen();
            const char* const error_message = all_texts[TEXT_LYNX_96K];
            print_encoded_str_at(1, 1, error_message);

            while (1)
            {
            }
        }
    }

    // 96k Lynx
    *(unsigned char*) 0xBFF8 = value;
}
