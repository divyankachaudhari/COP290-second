#ifndef CONSTANS_H
#define CONSTANS_H

#include "stdbool.h"

#define SCREEN_WIDTH 1280 // 2X
#define SCREEN_HEIGHT 960 // 2X
#define FRAME_TIME 16666
#define TILE_SIZE 64 // 2X
#define GRAVITY 1
#define MAX_VERTICAL_SPEED 25
#define PLAYER_JUMP_POWER 25
#define PLAYER_SPEED 4
#define PLAYER_WIDTH 32 // 2X
#define PLAYER_HEIGHT 32 // 2X
#define BULLET_HEIGHT 16 //16 // 2X
#define BULLET_WIDTH 16 //16 // 2X
#define MAX_PLAYERS 10
#define CLIENT_FRAME_TIME 15
#define SERVER_FRAME_TIME 15
#define X_AXIS 1
#define Y_AXIS 0
#define BUF_MAX 256
#define SPAWN_X 640
#define SPAWN_Y 480
#define LEFT_KEY 0x01
#define RIGHT_KEY 0x02
#define UP_KEY 0x04
#define DOWN_KEY 0x08
#define ATTACK_KEY 0x10

#endif
