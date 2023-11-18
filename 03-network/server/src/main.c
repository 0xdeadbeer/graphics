#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_error.h>
#include "structs.h"
#include "defs.h"

#define PORT 9080
#define DEBUG 1

int objects_count = 0;
struct object **objects_map;

int handle_player(void *data) {
    struct connection *connection_data = (struct connection *) data;
    struct object *obj = objects_map[connection_data->obj_id];

    memset(obj, 0, sizeof(struct object));

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

    for (;;) {
        char message[SERVER_MESSAGE_LEN]; 

        int ready_sockets = SDLNet_CheckSockets(set, 10); 
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

        if (DEBUG) 
            fprintf(stdout, "Notice: received '%s' and '%d' bytes\n", message, recv_len);
    
        // parsing the message
        uint8_t action = message[0];

        switch (action) {
            case RIGHT_MOVEMENT:
                obj->x += MOVEMENT_SPEED;
                break;
            case LEFT_MOVEMENT:
                obj->x -= MOVEMENT_SPEED;
                break;
            case JUMP_MOVEMENT:
                obj->force -= MOVEMENT_SPEED * 2.5;
                obj->colliding = 0;
                break;
            case CROUCH_MOVEMENT: // TODO 
                break;
            default: 
                break;
        }

    update_client:
        if (obj->y < 200) {
            obj->force += GRAVITY;
        } else {
            obj->colliding = 1;
            // obj->force = 0;
            obj->y = 200;
        }

        // gravity 
        obj->y += obj->force;

        // Update the client with information
        // Note: the order OBVIOUSLY matters
        int reply[] = { 
            obj->x, 
            obj->y,
            obj->colliding,
            obj->force
        };

        if (DEBUG) 
            fprintf(stdout, "Notice: new player properties (%d, %d, %d, %d)\n", reply[0], reply[1], reply[2], reply[3]);

        SDLNet_TCP_Send(connection_data->socket, reply, sizeof(int)*CLIENT_MESSAGE_LEN);
    }

    SDLNet_FreeSocketSet(set);
    SDLNet_TCP_Close(connection_data->socket);
    free(obj);
    free(data);

    return 0;
}

void catch_alarm(int sig) {
    fprintf(stdout, "Notice: force stopping server...\n");
    exit(EXIT_SUCCESS);
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

        objects_map = (struct object **) realloc(objects_map, sizeof(struct object *)*(objects_count+1));
        if (objects_map == NULL) {
            fprintf(stderr, "Error: failed reallocating objects hash map\n");
            return -1;
        }

        struct object *new_object = (struct object *) calloc(1, sizeof(struct object));
        if (new_object == NULL) {
            fprintf(stderr, "Error: failed allocating memory for new object\n");
            return -1;
        }

        objects_map[objects_count] = new_object;

        struct connection *new_connection = (struct connection *) calloc(1, sizeof(struct connection));
        if (new_connection == NULL) {
            fprintf(stderr, "Error: failed allocating memory for new connection object\n");
            return -1;
        }

        new_object->id = objects_count;
        new_connection->socket = client;
        new_connection->obj_id = objects_count;

        objects_count++;

        SDL_CreateThread(handle_player, "client", new_connection);
    }

    return 0;
}
