#include "room.h"
#include <malloc.h>
#include <string.h>

#include "../generated/level_data.h"
#include "../generated/texts_data.h"
#include "doors.h"
#include "objects.h"
#include "print.h"
#include "text_zone.h"
#include "tiles.h"
#include "utils.h"

#define ROOM_WIDTH 10
#define ROOM_HEIGHT 10

#define ROOM_BYTE_SIZE (ROOM_WIDTH * ROOM_HEIGHT)

typedef struct Room {
    room_id_t id;// Room ID is also it's position (0xXY) in the map
    unsigned char* graph_data;
} Room;

typedef enum PropertyBit
{
    PropertyBit_Blocking = 0x00,
    PropertyBit_Door = 0x01,
    PropertyBit_Unknown1 = 0x02,
    PropertyBit_Unknown2 = 0x04,
    PropertyBit_Unknown3 = 0x08,
    PropertyBit_Unknown4 = 0x10,
    PropertyBit_Unknown5 = 0x20,
    PropertyBit_Unknown6 = 0x40,
    PropertyBit_Unknown7 = 0x80,
} PropertyBit;

Room current_room;

void initialize_rooms()
{
    current_room.graph_data = malloc(ROOM_BYTE_SIZE);
}

void change_room(room_id_t room_id)
{
    current_room.id = room_id;

    // The compressed stream is in a RLE format.

    const unsigned char* compressed_stream = all_rooms[room_id];
    unsigned char* output = current_room.graph_data;
    unsigned char* output_end = output + ROOM_BYTE_SIZE;// The output is ROOM_BYTE_SIZE bytes long.

    while (output < output_end)
    {
        unsigned char byte = *compressed_stream;
        compressed_stream++;

        if (byte & 0x80)
        {
            // If the byte 7th bit is set, then the 7 low bits indicated the next byte repetition.
            unsigned char count = byte & 0x7F;
            byte = *compressed_stream;
            compressed_stream++;

            for (unsigned char i = 0; i < count; i++)
            {
                *output = byte;
                output++;
            }
        }
        else
        {
            // If the byte 7th bit is not set, then the byte is copied as is.
            *output = byte;
            output++;
        }
    }

    // Patch the doors activated by levers
    // TODO: copy the room data to a temporary buffer and patch it
    patch_level_with_doors(room_id, current_room.graph_data);
}

room_id_t get_current_room_id()
{
    return current_room.id;
}

const char* get_text_for_room(room_id_t id)
{
    for (size_t i = 0; i < level_to_text_count; i++)
    {
        const level_text_t* level_text = &level_to_text[i];
        if (level_text->level_id == id)
        {
            return all_texts[level_text->text_id];
        }
    }
    return NULL;
}

void display_room()
{
    const unsigned char* tile_pointer = current_room.graph_data;
    for (char y = 0; y < ROOM_HEIGHT * 2; y += 2)
    {
        for (char x = 0; x < ROOM_WIDTH * 2; x += 2)
        {
            unsigned char tile = *tile_pointer;
            display_tile(tile, x, y);
            tile_pointer++;
        }
    }

    display_objects(current_room.id);

    // Display text for room
    const char* room_text = get_text_for_room(current_room.id);
    if (room_text != NULL)
    {
        size_t text_length = strlen(room_text);
        clear_line(20);
        print_encoded_str(room_text);
        clear_line(21);

        // TODO: could be merged with clear_line
        const char dash = convert_char('-');
        for (size_t i = 0; i < text_length; i++)
        {
            put_char(dash);
        }
    }
    else
    {
        clear_line(20);
        clear_line(21);
    }
}

unsigned char get_room_tile_property_at(unsigned char position)
{
    const unsigned char* tile_pointer = current_room.graph_data;
    unsigned char tile_id = tile_pointer[position];

    return get_tile_property(tile_id);
}

unsigned char get_room_tile_id_at(unsigned char position)
{
    const unsigned char* tile_pointer = current_room.graph_data;
    return tile_pointer[position];
}

bool property_is_blocking(unsigned char property)
{
    return property & (1 << PropertyBit_Blocking);
}

room_id_t get_neighbour_room_id(room_id_t id, unsigned char* position)
{
    room_id_t id1 = id;
    const unsigned char levelWidthInRooms = level_data_max_x - level_data_min_x + 1;

    char x;
    char y;
    position_to_x_y(*position, &x, &y);

    if (x == 0)
    {
        x = ROOM_WIDTH - 2;
        id1 -= 1;
    }
    else if (x == ROOM_WIDTH - 1)
    {
        x = 1;
        id1 += 1;
    }
    else if (y == 0)
    {
        y = ROOM_HEIGHT - 2;
        id1 -= levelWidthInRooms;
    }
    else if (y == ROOM_HEIGHT - 1)
    {
        y = 1;
        id1 += levelWidthInRooms;
    }

    *position = x_y_to_position(x, y);

    return id1;
}

void position_to_x_y(unsigned char position, char* x, char* y)
{
    *x = position % ROOM_WIDTH;
    *y = position / ROOM_WIDTH;
}

unsigned char x_y_to_position(char x, char y)
{
    return y * ROOM_WIDTH + x;
}

unsigned char move_position_with_direction(unsigned char position, EMoveDirection direction)
{
    unsigned char x = position % ROOM_WIDTH;
    unsigned char y = position / ROOM_WIDTH;

    switch (direction)
    {
        case EMoveDirection_Up:
            y--;
            break;
        case EMoveDirection_Down:
            y++;
            break;
        case EMoveDirection_Left:
            x--;
            break;
        case EMoveDirection_Right:
            x++;
            break;
        default:
            break;
    }

    x = bound_char(x, 0, ROOM_WIDTH);
    y = bound_char(y, 0, ROOM_HEIGHT);

    return y * ROOM_WIDTH + x;
}
