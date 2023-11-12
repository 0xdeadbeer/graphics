#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "structs.h"

const int SCREEN_WIDTH = 640; 
const int SCREEN_HEIGHT = 480;
const Uint32 WINDOW_FLAGS = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS; 
const Uint32 RENDERER_FLAGS = SDL_RENDERER_ACCELERATED;
int GAME_RUNNING = 1;
struct game game; 
int movement_speed = 5;
int animation_frames = 100;
int frames = 0;

void prepare_scene(void) {
    SDL_SetRenderDrawColor(game.renderer, 96, 128, 255, 255);
    SDL_RenderClear(game.renderer);
}

void key(SDL_KeyboardEvent *event) {
    if (event->repeat != 0)
        return;   

    if (event->keysym.scancode == SDL_SCANCODE_W)
        game.up = !game.up;
    if (event->keysym.scancode == SDL_SCANCODE_S)
        game.down = !game.down;
    if (event->keysym.scancode == SDL_SCANCODE_A)
        game.left = !game.left;
    if (event->keysym.scancode == SDL_SCANCODE_D)
        game.right = !game.right;
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

struct object *create_object(const char *texture_path, int x, int y, int scale, int resolution) {
    struct object *object = (struct object *) calloc(1, sizeof(struct object));

    if (object == NULL) {
        fprintf(stderr, "Error: failed allocating memory for new object\n");
        return NULL;
    }

    object->texture = load_texture(texture_path);

    if (object->texture == NULL) {
        fprintf(stderr, "Error: loading texture into new object\n");
        return NULL;
    }

    object->x = x; 
    object->y = y;
    object->scale = scale;
    object->resolution = resolution;

    return object;
}

void draw_object(struct object *object) {
    SDL_Rect src;
    SDL_Rect dest;

    src.x = object->state * object->resolution;
    src.y = 0;

    src.w = object->resolution;
    src.h = object->resolution;

    dest.x = object->x; 
    dest.y = object->y; 
    dest.w = object->resolution * object->scale;
    dest.h = object->resolution * object->scale;

    SDL_RenderCopy(game.renderer, object->texture, &src, &dest);
}

void present_scene(void) {
    SDL_RenderPresent(game.renderer);
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error: could not initialize SDL\n%s\n", SDL_GetError());
        return -1;
    }

    game.window = SDL_CreateWindow("01-introduction", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_FLAGS);

    if (game.window == NULL) {
        fprintf(stderr, "Error: could not create window\n%s\n", SDL_GetError());
        return -1;
    }

    game.renderer = SDL_CreateRenderer(game.window, -1, RENDERER_FLAGS);    

    if (game.renderer == NULL) {
        fprintf(stderr, "Error: could not create renderer\n%s\n", SDL_GetError());
        return -1;
    }

    struct object *player = create_object("assets/player.png", SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 3, 48);

    // game loop
    while (GAME_RUNNING) {
        frames++;
        prepare_scene();
        handle_input(); 

        if (game.up)
            player->y -= movement_speed; 
        if (game.down)
            player->y += movement_speed;
        if (game.left) 
            player->x -= movement_speed; 
        if (game.right) 
            player->x += movement_speed;

        player->state = ((frames % animation_frames)/5) % 10;

        draw_object(player);

        present_scene();

        SDL_Delay(16);
    }

    SDL_DestroyWindow(game.window);
    SDL_Quit();

    return 0; 
}
