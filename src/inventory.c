#include "inventory.h"

#include <stdbool.h>
#include <string.h>

#include "../generated/inventory_data.h"
#include "../generated/objects_data.h"
#include "tiles.h"

typedef struct
{
    unsigned char tile_id;
    unsigned char position;
} InventoryEntry;

#define INVENTORY_SIZE 12
unsigned char current_inventory[INVENTORY_SIZE] = {0};

void initialize_inventory()
{
    memset(current_inventory, 0, INVENTORY_SIZE);

    // Debug
    // Add all objects to inventory
//    for (size_t i = 0; i < 120; i++)
//    {
//        add_to_inventory(i);
//    }
}

#define INVENTORY_X 21
#define INVENTORY_Y 4
#define INVENTORY_WIDTH 5

void display_inventory()
{
    for (size_t i = 0; i < INVENTORY_SIZE; i++)
    {
        unsigned char tile_id = current_inventory[i];
        if (tile_id != 0)
        {
            // Coordinates
            char x = INVENTORY_X + ((i % INVENTORY_WIDTH) * 2);
            char y = INVENTORY_Y + ((i / INVENTORY_WIDTH) * 2);

            copy_tile_16_at(get_tile_graph(tile_id), x, y);
        }
    }
}

void add_to_inventory(unsigned char tile_id)
{
    bool object_added = false;

    InventoryEntry* inventory_entries = (InventoryEntry*) inventory;
    for (size_t i = 0; i < inventory_count; i++)
    {
        InventoryEntry* entry = &inventory_entries[i];
        if (entry->tile_id == tile_id)
        {
            unsigned char position = entry->position;
            current_inventory[position] = tile_id;
            object_added = true;
        }
    }

    if (object_added)
    {
        display_inventory();
    }
}

bool is_in_inventory(unsigned char tile_id)
{
    for (size_t i = 0; i < INVENTORY_SIZE; i++)
    {
        if (current_inventory[i] == tile_id)
        {
            return true;
        }
    }

    return false;
}

bool has_secret_been_found()
{
    return is_in_inventory(hidden_treasure_tile_id);
}
