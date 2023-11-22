#ifndef JEU_LYNX_TELEPORT_H
#define JEU_LYNX_TELEPORT_H

typedef struct {
    unsigned char source_room_id;
    unsigned char source_position;
    unsigned char destination_room_id;
    unsigned char destination_position;
} Teleport;

const Teleport* get_teleport_at(unsigned char room_id, unsigned char position);

#endif
