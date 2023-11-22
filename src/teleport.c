#include "teleport.h"

#include "../generated/links_data.h"
#include "room.h"

#include <stddef.h>

const Teleport* get_teleport_at(unsigned char room_id, unsigned char position)
{
    const Teleport *all_teleport = (Teleport*) links;
    for (size_t i = 0; i < links_count; i++)
    {
        const Teleport* teleport = &all_teleport[i];
        if (teleport->source_room_id == room_id &&
            teleport->source_position == position)
        {
            return teleport;
        }
    }

    return NULL;
}
