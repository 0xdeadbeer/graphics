#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include "structs.h"
#include "defs.h"
#include "format.h"

#define DEBUG 1

int game_running = 1;
struct game game; 
int movement_speed = 10;
double delta_time; 
double last_frame;

// network
TCPsocket server_socket;

// textures
SDL_Texture *idle_animation;
SDL_Texture *move_left_animation;
SDL_Texture *move_right_animation;
SDL_Texture *move_up_animation;
SDL_Texture *move_down_animation;
SDL_Texture *idle_left_animation;
SDL_Texture *idle_right_animation;
SDL_Texture *idle_up_animation;
SDL_Texture *idle_down_animation;

// scene
int objects_count = 0;
struct object ** objects_map;

void prepare_scene(void) {
    SDL_SetRenderDrawColor(game.renderer, 96, 128, 255, 255);
    SDL_RenderClear(game.renderer);
}

void key(SDL_KeyboardEvent *event) {
    if (event->repeat != 0)
        return;   

    if (event->keysym.scancode == SDL_SCANCODE_D)
        game.left = !game.left;
    if (event->keysym.scancode == SDL_SCANCODE_F)
        game.right = !game.right;
    if (event->keysym.scancode == SDL_SCANCODE_K)
        game.up = !game.up;
    if (event->keysym.scancode == SDL_SCANCODE_J)
        game.down = !game.down;
}

void handle_input(void) {
    SDL_Event event; 

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: 
                game_running = 0; 
                break;
            case SDL_KEYDOWN: 
            case SDL_KEYUP: 
                key(&event.key); 
                break;
            default:
                break;
        }
        if (event.type == SDL_QUIT) 
            game_running = 0;
    }
}

SDL_Texture *load_texture(const char *path) {
    SDL_Texture *texture; 
    
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", path);
    
    texture = IMG_LoadTexture(game.renderer, path);

    return texture;
}

void present_scene(void) {
    SDL_RenderPresent(game.renderer);
}

int new_key_position(void **map, int slots) {
    int slot = 0; 

    if (slots <= 0) 
        return slot; 

    if (map == NULL) 
        return slot; 

    while (slot <= slots) {
        if (map[slots] == NULL) 
            break;

        slot++;
    }

    return slot;
}

int handle_server(void *data) {
    for (;;) {
        int message[CLIENT_MESSAGE_LEN];
        int recv_len = SDLNet_TCP_Recv(server_socket, message, sizeof(int)*CLIENT_MESSAGE_LEN);

        if (!recv_len) {
            fprintf(stderr, "Error: failed receiving message from server\n%s\n", SDLNet_GetError());
            break;
        }

        if (DEBUG) 
            fprintf(stdout, "Notice: received '%d' bytes from server\n", recv_len);

        if (message[0] == PLAYER_CONNECT_FORMAT) {
            if (DEBUG) 
                fprintf(stdout, "DEBUG: PLAYER CONNECT MESSAGE\n");

            int ret = handle_player_connect(message, &objects_map, &objects_count);
            if (ret == MEMERR) {
                fprintf(stderr, "MEMERR: Failed handling memory for player connect\n");
                return MEMERR;
            }

            continue; 
        }

        if (message[0] == OBJECT_PROPERTIES_FORMAT) {
            if (DEBUG) 
                fprintf(stdout, "DEBUG: OBJECT PROPERTIES MESSAGE FOR ID %d\n", message[1]);
            
            int ret = handle_object_properties(message, &objects_map, &objects_count);
            if (ret == MEMERR) {
                fprintf(stderr, "MEMERR: Failed handling memory for new object properties\n");
                return MEMERR;
            }

            continue; 
        }

        if (message[0] == PLAYER_DISCONNECT_FORMAT) {
            if (DEBUG) 
                fprintf(stdout, "DEBUG: PLAYER DISCONNECT MESSAGE\n");

            int object_id = message[1]; 

            free(objects_map[object_id]);
            objects_map[object_id] = NULL; 

            continue;
        }
    }

    return 0; 
}

int connect_to_server(void) {
    IPaddress ip; 
    if (SDLNet_ResolveHost(&ip, SERVER_ADDR, SERVER_PORT) != 0) {
        fprintf(stderr, "Error: resolving host of server\n%s\n", SDLNet_GetError());
        return -1;
    }

    server_socket = SDLNet_TCP_Open(&ip);
    if (!server_socket) {
        fprintf(stderr, "Error: failed opening socket to '%s' at '%d'\n%s\n", SERVER_ADDR, SERVER_PORT, SDLNet_GetError());
        return -1;
    }

    SDL_CreateThread(handle_server, "server", NULL);

    return 0;
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error: could not initialize SDL\n%s\n", SDL_GetError());
        return -1;
    }

    if (SDLNet_Init() != 0) {
        fprintf(stderr, "Error: could not initialize SDL net\n%s\n", SDL_GetError());
        return -1;
    }

    game.window = SDL_CreateWindow("03-network", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_FLAGS);

    if (game.window == NULL) {
        fprintf(stderr, "Error: could not create window\n%s\n", SDL_GetError());
        return -1;
    }

    game.renderer = SDL_CreateRenderer(game.window, -1, RENDERER_FLAGS);

    if (game.renderer == NULL) {
        fprintf(stderr, "Error: could not create renderer\n%s\n", SDL_GetError());
        return -1;
    }

    // load player animations 
    move_left_animation = load_texture("assets/player/move_left.png");
    move_right_animation = load_texture("assets/player/move_right.png");
    move_up_animation = load_texture("assets/player/move_up.png");
    move_down_animation = load_texture("assets/player/move_down.png");
    idle_left_animation = load_texture("assets/player/idle_left.png");
    idle_right_animation = load_texture("assets/player/idle_right.png");
    idle_up_animation = load_texture("assets/player/idle_up.png");
    idle_down_animation = load_texture("assets/player/idle_down.png");
    idle_animation = idle_down_animation;

    if (connect_to_server() != 0)
        return -1;

    // game loop
    while (game_running) {
        double current_frame = SDL_GetTicks();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        prepare_scene();
        handle_input(); 

        uint8_t message[SERVER_MESSAGE_LEN];
        message[0] = (game.left << 7)  | 
                     (game.right << 6) |
                     (game.up << 5)    | 
                     (game.down << 4);

        SDLNet_TCP_Send(server_socket, message, sizeof(uint8_t)*SERVER_MESSAGE_LEN);

        for (int iter = 0; iter < objects_count; iter++) {
            struct object *obj = objects_map[iter];
            if (obj == NULL) 
                continue; 

            if (obj->state & LEFT_MOVEMENT) {
                switch_animation(obj, move_left_animation); 
                idle_animation = idle_left_animation;
            }

            if (obj->state & RIGHT_MOVEMENT) {
                switch_animation(obj, move_right_animation); 
                idle_animation = idle_right_animation;
            }

            if (obj->state & UP_MOVEMENT) {
                switch_animation(obj, move_up_animation);
                idle_animation = idle_up_animation;
            }

            if (obj->state & DOWN_MOVEMENT) {
                switch_animation(obj, move_down_animation);
                idle_animation = idle_down_animation;
            }

            if (obj->state == NO_MOVEMENT) 
                switch_animation(obj, idle_animation);

            obj->animation_speed = 6;

            draw_object(&game, obj);
        }

        present_scene();

        SDL_Delay(16);
    }

    SDL_DestroyWindow(game.window);
    SDL_Quit();

    return 0; 
}
