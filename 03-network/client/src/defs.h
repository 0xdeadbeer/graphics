#ifndef DEFS_H
#define DEFS_H

#include <SDL2/SDL.h>

// Server details
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 9080

// ERRORS 
#define STDOK 0
#define STDERR -1 
#define MEMERR -2

// SDL2 
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 480
#define WINDOW_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED

// MCS: Message Communication Standard
#define SERVER_MESSAGE_LEN 10
#define CLIENT_MESSAGE_LEN 7

// MFS: Message Format Standard
#define PLAYER_CONNECT_FORMAT 0
#define OBJECT_PROPERTIES_FORMAT 1
#define PLAYER_DISCONNECT_FORMAT 2

// MSCA: Message Communication Standard Actions
#define LEFT_MOVEMENT   0b10000000
#define RIGHT_MOVEMENT  0b01000000
#define JUMP_MOVEMENT   0b00100000
#define CROUCH_MOVEMENT 0b00010000

#endif 
