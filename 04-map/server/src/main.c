#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_error.h>
#include "defs.h"
#include "structs.h"
#include "hashmap.h"

#define DEBUG 0x0010
#define PORT 9080
#define MAP_WIDTH 25
#define MAP_HEIGHT 25

struct connection **connections_map;
int connections_map_size = 0; 
int connections_count = 0; 

int handle_connection(void *data) {
    struct connection *connection_data = (struct connection *) data;

    struct map *new_map = generate_map(MAP_WIDTH, MAP_HEIGHT);    
    if (new_map == NULL) {
        fprintf(stderr, "Erorr: failed generating map\n");
        goto end;
    }

    SDLNet_TCP_Send(connection_data->socket, new_map->blocks, new_map->width*new_map->height*sizeof(int));
end:

    SDLNet_TCP_Close(connection_data->socket);
    connections_map[connection_data->id] = NULL;
    connections_count--;
    free(data);

    return 0;
}

void catch_alarm(int sig) {
    fprintf(stdout, "Notice: force stopping server...\n");
    exit(EXIT_SUCCESS);
}

int new_key_position(void **map, int slots) {
    int slot = 0; 

    if (slots <= 0) 
        return slot;

    if (map == NULL) 
        return slot;

    while (slot <= slots) {
        if (map[slot] == NULL)
            break;
        
        slot++;
    }

    return slot;
}

int main(int argc, char *argv[]) {
    signal(SIGINT, catch_alarm);
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error: could not initialize SDL\n%s\n", SDL_GetError());
        return -1;
    }

    if (SDLNet_Init() != 0) {
        fprintf(stderr, "Error: could not initialize SDL net\n%s\n", SDL_GetError());
        return -1;
    }

    IPaddress ip; 
    if (SDLNet_ResolveHost(&ip, NULL, PORT) == -1) {
        fprintf(stderr, "Error: failed resolving host \n%s\n", SDL_GetError());
        return -1;
    }

    TCPsocket server = SDLNet_TCP_Open(&ip); 
    if (!server) {
        fprintf(stderr, "Error: failed opening socket at %d\n%s\n", PORT, SDL_GetError());
        return -1;
    }

    for (;;) {
        TCPsocket client = SDLNet_TCP_Accept(server);
        if (!client) {
            SDL_Delay(100); 
            continue;
        }

        fprintf(stdout, "Notice: accepted a connection from client!\n"); 

        int new_connection_slot = allocate_value((void ***) &connections_map, &connections_map_size, &connections_count, sizeof(struct connection));
        if (new_connection_slot == MEMERR) {
            fprintf(stderr, "MEMERR: failed allocating memory for new connection\n");
            return STDERR;
        }

        struct connection *new_connection = connections_map[new_connection_slot];

        new_connection->id = new_connection_slot;
        new_connection->socket = client;

        if (DEBUG & PLAYER_DEBUG) 
            fprintf(stdout, "Created connection with id '%d'\n", new_connection->id);

        SDL_CreateThread(handle_connection, "client", new_connection);
    }

    return 0;
}
