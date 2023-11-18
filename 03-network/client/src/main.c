#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include "structs.h"

#define DEBUG 1
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 9080
#define SERVER_MESSAGE_LEN 10 // 0-1 BYTE: Action details, 2-9: Other data (not sure we even need this)
#define CLIENT_MESSAGE_LEN 4 // 4 PROPERTIES (each of 4 BYTES)

const int SCREEN_WIDTH = 1000; 
const int SCREEN_HEIGHT = 480;
const Uint32 WINDOW_FLAGS = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL; 
const Uint32 RENDERER_FLAGS = SDL_RENDERER_ACCELERATED;
int GAME_RUNNING = 1;
struct game game; 
int movement_speed = 10;
double delta_time; 
double last_frame;

// network
TCPsocket server_socket;

// scene 
struct object *player;

void prepare_scene(void) {
    SDL_SetRenderDrawColor(game.renderer, 96, 128, 255, 255);
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
        switch (event.type) {
            case SDL_QUIT: 
                GAME_RUNNING = 0; 
                break;
            case SDL_KEYDOWN: 
            case SDL_KEYUP: 
                key(&event.key); 
                break;
            default:
                break;
        }
        if (event.type == SDL_QUIT) {
            GAME_RUNNING = 0;
        }
    }
}

SDL_Texture *load_texture(const char *path) {
    SDL_Texture *texture; 
    
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", path);
    
    texture = IMG_LoadTexture(game.renderer, path);

    return texture;
}

void switch_animation(struct object *object, SDL_Texture *animation) {
    if (object->texture == animation)
        return;

    object->animation_slide = 0; 
    object->texture = animation;
}

struct object *create_object(SDL_Texture *texture, int scale, int resolution) {
    struct object *object = (struct object *) calloc(1, sizeof(struct object));

    if (object == NULL) {
        fprintf(stderr, "Error: failed allocating memory for new object\n");
        return NULL;
    }

    object->texture = texture;
    object->resolution = resolution;
    object->animation_speed = 13;
    object->scale = scale;

    return object;
}

void draw_object(struct object *object) {
    int texture_width; 
    SDL_Rect src;
    SDL_Rect dest;

    src.x = object->animation_slide * object->resolution;
    src.y = 0;

    src.w = object->resolution;
    src.h = object->resolution;

    dest.x = object->x; 
    dest.y = object->y; 
    dest.w = object->resolution * object->scale;
    dest.h = object->resolution * object->scale;

    SDL_RenderCopyEx(game.renderer, object->texture, &src, &dest, 0, NULL, object->flip);

    // update animation slide
    SDL_QueryTexture(object->texture, NULL, NULL, &texture_width, NULL);

    object->animation_clock += object->animation_speed*(delta_time/1000);

    if (object->animation_clock >= 1) {
        object->animation_clock = 0;    
        object->animation_slide = (object->animation_slide+1) % (texture_width / object->resolution); // clock arithmetic: jump back to first animation slide 
    }
}

void present_scene(void) {
    SDL_RenderPresent(game.renderer);
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

        player->x = message[0];
        player->y = message[1];
        player->colliding = message[2];
        player->force = message[3];
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
    SDL_Texture *idle_animation = load_texture("assets/player/idle.png");
    SDL_Texture *jump_animation = load_texture("assets/player/jump.png");
    SDL_Texture *run_animation = load_texture("assets/player/run.png");

    player = create_object(idle_animation, 4, 48);

    if (connect_to_server() != 0)
        return -1;

    // game loop
    while (GAME_RUNNING) {
        double current_frame = SDL_GetTicks();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        prepare_scene();
        handle_input(); 

        // The reason why I don't do 
        // that much game development 
        // in my free time. Damn youuu ifs
        if (player->colliding) {
            if (game.left)
                switch_animation(player, run_animation);
            if (game.right) 
                switch_animation(player, run_animation);
            if (!game.left && !game.right) 
                switch_animation(player, idle_animation);
            if (game.jump) {
                switch_animation(player, jump_animation);

                uint8_t message[SERVER_MESSAGE_LEN];
                message[0] = 2;

                SDLNet_TCP_Send(server_socket, message, sizeof(uint8_t)*SERVER_MESSAGE_LEN);
            }
        }

        if (game.left) {
            player->flip = SDL_FLIP_HORIZONTAL;
            uint8_t message[SERVER_MESSAGE_LEN];
            message[0] = 1;

            SDLNet_TCP_Send(server_socket, message, sizeof(uint8_t)*SERVER_MESSAGE_LEN);
        }
        if (game.right) {
            player->flip = SDL_FLIP_NONE;
            uint8_t message[SERVER_MESSAGE_LEN];
            message[0] = 0;

            SDLNet_TCP_Send(server_socket, message, sizeof(uint8_t)*SERVER_MESSAGE_LEN);
        }

        draw_object(player);

        present_scene();

        SDL_Delay(16);
    }

    SDL_DestroyWindow(game.window);
    SDL_Quit();

    return 0; 
}
