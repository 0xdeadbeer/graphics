#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_error.h>
#include "structs.h"
#include "defs.h"

#define DEBUG 0x0010
#define PORT 9080

int objects_count = 0;
struct object **objects_map;

int connections_count = 0; 
struct connection **connections_map;

void handle_player_input(struct object *obj, char *message) {
    uint8_t action = message[0];

    if (action & RIGHT_MOVEMENT) 
        obj->x += MOVEMENT_SPEED;

    if (action & LEFT_MOVEMENT)
        obj->x -= MOVEMENT_SPEED; 

    if (action & JUMP_MOVEMENT) {
        obj->force -= MOVEMENT_SPEED; 
        obj->colliding = 0;
    }

    obj->state = action;
}

void handle_player_physics(struct object *obj) {
    if (obj->y < 200) 
        obj->force += GRAVITY; 
    else {
        obj->colliding = 1; 
        obj->y = 200;
    }

    obj->y += obj->force;
}

void send_player_positions(TCPsocket socket, int player_id) {
    if (objects_map == NULL)
        return;

    for (int iter = 0; iter < objects_count; iter++) {
        struct object *obj = objects_map[iter];
        if (obj == NULL) 
            continue;

        // Update the client with every object's information
        // Note: the order OBVIOUSLY matters
        int reply[] = {
            OBJECT_PROPERTIES_FORMAT,
            obj->id,
            obj->x,
            obj->y,
            obj->colliding,
            obj->force,
            obj->state
        };

        if (DEBUG & PLAYER_DEBUG) 
            fprintf(stdout, "PLAYER_DEBUG: player '%d' properties (%d, %d, %d, %d)\n", 
                    obj->id, obj->x, obj->y, obj->colliding, obj->force);

        SDLNet_TCP_Send(socket, reply, sizeof(int)*CLIENT_MESSAGE_LEN);
    }
}

int send_player_connect_event(int object_id) {
    struct object *obj = objects_map[object_id];

    for (int iter = 0; iter < connections_count; iter++) {
        struct connection *con = connections_map[iter]; 
        if (con == NULL) 
            continue; 

        int message[] = {
            PLAYER_CONNECT_FORMAT,
            obj->id,
            obj->x, 
            obj->y, 
            obj->colliding, 
            obj->force,
            obj->state
        };

        if (DEBUG & PLAYER_DEBUG)
            fprintf(stdout, "PLAYER_DEBUG: new player '%d' has connected to the server\n", obj->id);

        SDLNet_TCP_Send(con->socket, message, sizeof(int)*CLIENT_MESSAGE_LEN);
    }

    return 0;
}

int send_player_disconnect_event(int object_id) {
    struct object *obj = objects_map[object_id];

    for (int iter = 0; iter < connections_count; iter++) {
        struct connection *con = connections_map[iter]; 
        if (con == NULL) 
            continue; 

        int message[] = {
            PLAYER_DISCONNECT_FORMAT, 
            obj->id, 
            obj->x, 
            obj->y, 
            obj->colliding, 
            obj->force,
            obj->state
        };

        if (DEBUG & PLAYER_DEBUG)
            fprintf(stdout, "PLAYER_DEBUG: player '%d' has disconnected from the server\n", obj->id);

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

    send_player_connect_event(connection_data->obj_id); 

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
        send_player_positions(connection_data->socket, connection_data->obj_id);
    }

    // Communicate client disconnect
    send_player_disconnect_event(connection_data->obj_id);

    SDLNet_FreeSocketSet(set);
    SDLNet_TCP_Close(connection_data->socket);
    objects_map[connection_data->obj_id] = NULL;
    connections_map[connection_data->id] = NULL;
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

        int new_object_position = new_key_position((void **) objects_map, objects_count); 
        if (new_object_position == objects_count) {
            objects_map = (struct object **) realloc(objects_map, sizeof(struct object *)*objects_count);
            objects_count++;
        }
        if (objects_map == NULL) {
            fprintf(stderr, "Error: failed reallocating objects hash map\n");
            return -1;
        }

        struct object *new_object = (struct object *) calloc(1, sizeof(struct object));
        if (new_object == NULL) {
            fprintf(stderr, "Error: failed allocating memory for new object\n");
            return -1;
        }

        int new_connection_position = new_key_position((void **) connections_map, connections_count);
        if (new_connection_position == connections_count) {
            connections_map = (struct connection **) realloc(connections_map, sizeof(struct connection *)*connections_count);
            connections_count++;
        }
        if (connections_map == NULL) {
            fprintf(stderr, "Error: failed reallocating connections hash map\n");
            return -1;
        }

        struct connection *new_connection = (struct connection *) calloc(1, sizeof(struct connection));
        if (new_connection == NULL) {
            fprintf(stderr, "Error: failed allocating memory for new connection object\n");
            return -1;
        }

        objects_map[new_object_position] = new_object;
        connections_map[new_connection_position] = new_connection;
        new_object->id = new_object_position;
        new_connection->id = new_connection_position;
        new_connection->socket = client;
        new_connection->obj_id = new_object_position;

        if (DEBUG & PLAYER_DEBUG) 
            fprintf(stdout, "Notice: new player object id: %d\n", new_object_position);

        SDL_CreateThread(handle_player, "client", new_connection);
    }

    return 0;
}
