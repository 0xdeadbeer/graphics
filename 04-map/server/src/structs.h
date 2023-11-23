#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL_net.h>

struct connection {
    int id;
    TCPsocket socket;
};

struct map {
    int width; 
    int height; 
    int *blocks;
};

struct map *generate_map(int width, int height);

#endif
