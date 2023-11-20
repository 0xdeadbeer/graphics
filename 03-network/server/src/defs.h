#ifndef DEFS_H
#define DEFS_H

// DEBUG 
#define GENERAL_DEBUG   0x0001
#define PLAYER_DEBUG    0x0010

// ERRORS 
#define STDERR -1
#define MEMERR -2

// MCS: Message Communication Standard
#define SERVER_MESSAGE_LEN 10
#define CLIENT_MESSAGE_LEN 7

// MFS: Message Format Standard
#define PLAYER_CONNECT_FORMAT 0
#define OBJECT_PROPERTIES_FORMAT 1
#define PLAYER_DISCONNECT_FORMAT 2

// MCSA: Message Communication Standard Actions
#define LEFT_MOVEMENT   0b10000000
#define RIGHT_MOVEMENT  0b01000000
#define JUMP_MOVEMENT   0b00100000
#define CROUCH_MOVEMENT 0b00010000

// OC: Object Constants
#define GRAVITY 2
#define MOVEMENT_SPEED 10

#endif 
