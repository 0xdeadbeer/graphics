#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL_net.h>

struct connection {
    int id;
    int obj_id;
    TCPsocket socket;
};

struct object {
    int id;
    int x;
    int y;

    int colliding;
    int force;
    
    uint8_t state;
};

#endif
