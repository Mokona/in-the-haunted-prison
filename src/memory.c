#include "memory.h"

#include <malloc.h>

// Warning, heap first address seems to require an alignment.
// #define STACK_BASE 0x9fff  // Works for Lynx 96k
#define STACK_BASE 0xcfff // Works only for Lynx 96k -> In fact, it's to be set depending on the SP (RAM can be used by disk at the end. But do we care ?)
                          // If not, SP has to be reset
#define STACK_SIZE 0x100
#define HEAP_SIZE 1025
#define HEAP_ADDR (STACK_BASE - STACK_SIZE - HEAP_SIZE)

// heap pointer's location specified in RAM
long heap;
void init_malloc()
{
    mallinit();
    sbrk((void*)HEAP_ADDR, HEAP_SIZE);
}
