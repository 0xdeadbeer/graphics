#include <stdio.h>
#include <signal.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_mutex.h>
#include "structs.h"
#include "format.h"
#include "defs.h"

#define DEBUG 1
#define MAP_WIDTH 100
#define MAP_HEIGHT 100

int original_width = 1000;
int original_height = 480;
int screen_width = 1000; 
int screen_height = 480;
int game_running = 1;
struct game game; 
int movement_speed = 10;
double delta_time; 
double last_frame;

// network
TCPsocket server_socket;

// Textures
SDL_Texture *grass_texture; 
SDL_Texture *cobble_texture; 
SDL_Texture *lava_texture;

// scene
int objects_count = 0;
struct object ** objects_map;
SDL_mutex *objects_map_mutex;

void prepare_scene(void) {
    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 0);
    SDL_RenderClear(game.renderer);
}

void key(SDL_KeyboardEvent *event) {
    if (event->repeat != 0)
        return;   

    if (event->keysym.scancode == SDL_SCANCODE_J)
        game.left = !game.left;
    if (event->keysym.scancode == SDL_SCANCODE_K)
        game.right = !game.right;
    if (event->keysym.scancode == SDL_SCANCODE_SPACE)
        game.jump = !game.jump;
}

void handle_input(void) {
    SDL_Event event; 

    while (SDL_PollEvent(&event)) {
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
    int message[MAP_WIDTH*MAP_HEIGHT];
    int recv_len = SDLNet_TCP_Recv(server_socket, message, sizeof(int)*MAP_WIDTH*MAP_HEIGHT);

    if (!recv_len) {
        fprintf(stderr, "Error: failed receiving message from server\n%s\n", SDLNet_GetError());
        return MEMERR;
    }

    if (DEBUG) 
        fprintf(stdout, "Notice: received '%d' bytes from server\n", recv_len);

    SDL_LockMutex(objects_map_mutex);

    int blocks_num = recv_len / sizeof(int);

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {

            int iter = (y*MAP_WIDTH)+x;
            int ret = handle_block_creation(iter, message[iter], &objects_map, &objects_count);
            if (ret == MEMERR) {
                fprintf(stderr, "Error: failed creating new block\n");
                return MEMERR;
            }

            struct object *block = objects_map[iter];
            block->x = x * block->resolution * block->scale;
            block->y = y * block->resolution * block->scale;
        }
    }

    SDL_UnlockMutex(objects_map_mutex);

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

void catch_alarm(int sig) {
    exit(EXIT_FAILURE);
}

void handle_events() {
    SDL_Event event; 

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT: 
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) 
                    SDL_GetWindowSize(game.window, &screen_width, &screen_height);
                break;
            case SDL_QUIT: 
                exit(EXIT_SUCCESS);
                break;
        }
    }
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

    game.window = SDL_CreateWindow("03-network", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, WINDOW_FLAGS);
    SDL_SetWindowResizable(game.window, 1);

    if (game.window == NULL) {
        fprintf(stderr, "Error: could not create window\n%s\n", SDL_GetError());
        return -1;
    }

    game.renderer = SDL_CreateRenderer(game.window, -1, RENDERER_FLAGS);

    if (game.renderer == NULL) {
        fprintf(stderr, "Error: could not create renderer\n%s\n", SDL_GetError());
        return -1;
    }

    // load textures
    grass_texture = load_texture("assets/floor/grass.png");
    cobble_texture = load_texture("assets/floor/cobble.png");
    lava_texture = load_texture("assets/floor/lava.png");

    // create mutexes 
    objects_map_mutex = SDL_CreateMutex();

    if (connect_to_server() != 0)
        return -1;

    while (game_running) {
        prepare_scene();

        handle_events();

        int lock = SDL_TryLockMutex(objects_map_mutex); 
        if (lock == 0) {
            for (int iter = 0; iter < objects_count; iter++) {
                struct object *object = objects_map[iter];
                if (object == NULL)
                    continue; 

                object->x = object->x - (object->scale * object->resolution * (original_width - original_width));
                object->y = object->y - (object->scale * object->resolution * (original_height - original_height));

                draw_object(&game, object);
            }

            SDL_UnlockMutex(objects_map_mutex);
        }

        present_scene(); 

        SDL_Delay(100);
    }

    SDL_DestroyWindow(game.window);
    SDL_Quit();

    return 0; 
}
