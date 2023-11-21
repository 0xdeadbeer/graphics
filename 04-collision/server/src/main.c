#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_error.h>
#include "defs.h"
#include "structs.h"
#include "structures.h"

#define DEBUG 0x0010
#define PORT 9080

struct object **objects_map;
int objects_map_size = 0;
int objects_count = 0;

struct connection **connections_map;
int connections_map_size = 0; 
int connections_count = 0; 

void handle_player_input(struct object *obj, char *message) {
    uint8_t action = message[0];

    if (action & LEFT_MOVEMENT)
        obj->x -= MOVEMENT_SPEED; 

    if (action & RIGHT_MOVEMENT) 
        obj->x += MOVEMENT_SPEED;

    if (action & UP_MOVEMENT) 
        obj->y -= MOVEMENT_SPEED; 

    if (action & DOWN_MOVEMENT) 
        obj->y += MOVEMENT_SPEED; 

    obj->state = action;
}

void handle_player_physics(struct object *obj) {
}

int broadcast_event(int format, int object_id) {
    struct object *obj = objects_map[object_id];

    for (int iter = 0; iter < connections_map_size; iter++) {
        struct connection *con = connections_map[iter]; 
        if (con == NULL) 
            continue; 

        int message[] = {
            format, 
            obj->id, 
            obj->x, 
            obj->y, 
            obj->colliding, 
            obj->force,
            obj->state
        };

        SDLNet_TCP_Send(con->socket, message, sizeof(int)*CLIENT_MESSAGE_LEN);
    }

    return 0;
}

int handle_player(void *data) {
    struct connection *connection_data = (struct connection *) data;
    struct object *obj = objects_map[connection_data->obj_id];

    SDLNet_SocketSet set = SDLNet_AllocSocketSet(1);
    if (set == NULL) {
        fprintf(stderr, "Error: cannot allocate memory for a new socket set\n");
        return -1;
    }

    int ret = SDLNet_TCP_AddSocket(set, connection_data->socket);
    if (ret == -1) {
        fprintf(stderr, "Error: max socket count in socket set reached\n");
        return -1;
    }

    broadcast_event(PLAYER_CONNECT_FORMAT, connection_data->obj_id);

    for (;;) {
        char message[SERVER_MESSAGE_LEN]; 
        int ready_sockets = SDLNet_CheckSockets(set, 100); 
        if (ready_sockets == -1) {
            fprintf(stderr, "Error: cannot call select() system call with SDLNet_CheckSockets()\n");
            continue;
        } 
        
        if (ready_sockets == 0)
            goto update_client;

        int recv_len = SDLNet_TCP_Recv(connection_data->socket, message, SERVER_MESSAGE_LEN);
        if (!recv_len) { 
            // player disconnected
            fprintf(stderr, "Error: failed receiving message\n%s\n", SDLNet_GetError());
            break;
        }

        if (DEBUG & GENERAL_DEBUG) 
            fprintf(stdout, "Notice: received '%s' and '%d' bytes\n", message, recv_len);
    
        handle_player_input(obj, message);

    update_client:
        handle_player_physics(obj);
        broadcast_event(OBJECT_PROPERTIES_FORMAT, connection_data->obj_id);
    }

    // Communicate client disconnect
    broadcast_event(PLAYER_DISCONNECT_FORMAT, connection_data->obj_id);

    SDLNet_FreeSocketSet(set);
    SDLNet_TCP_Close(connection_data->socket);

    objects_map[connection_data->obj_id] = NULL;
    objects_count--;

    connections_map[connection_data->id] = NULL;
    connections_count--;

    free(obj);
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

        int new_object_slot = map_allocate_value((void ***) &objects_map, &objects_map_size, &objects_count, sizeof(struct object));
        if (new_object_slot == MEMERR) {
            fprintf(stderr, "MEMERR: failed allocating memory for new object\n");
            return STDERR;
        }

        int new_connection_slot = map_allocate_value((void ***) &connections_map, &connections_map_size, &connections_count, sizeof(struct connection));
        if (new_connection_slot == MEMERR) {
            fprintf(stderr, "MEMERR: failed allocating memory for new connection\n");
            return STDERR;
        }

        struct object *new_object = objects_map[new_object_slot];
        struct connection *new_connection = connections_map[new_connection_slot];

        new_object->id = new_object_slot;
        new_connection->id = new_connection_slot;
        new_connection->socket = client;
        new_connection->obj_id = new_object_slot;

        if (DEBUG & PLAYER_DEBUG) 
            fprintf(stdout, "Created connection with id '%d' and object '%d'\n", new_connection->id, new_object->id);

        SDL_CreateThread(handle_player, "client", new_connection);
    }

    return 0;
}
