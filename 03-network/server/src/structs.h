#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL_net.h>

struct connection {
    TCPsocket socket;
    int obj_id;
};

struct object {
    int id;
    int x;
    int y;

    int colliding;
    int force;
};

#endif
