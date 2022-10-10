/*
 * DO NOT CHANGE THIS FILE!
 */

#include <ncurses.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include "string.h"
#include <time.h>

// Map geometry
#define MAP_WIDTH 50
#define MAP_LENGTH 50

// Field Types
#define EMPTY 1
#define PLAYER 2
#define SUPPLY_CRATE 3
#define ENEMY_ALIEN 6
#define ENEMY_BLACKHOLE 7

// ncurses constants
#define SPACE 51
#define PLAYER_COLOR 52
#define ALIEN_COLOR 53
#define CRATE_COLOR 54
#define BLACKHOLE_COLOR 55

// Game settings
#define POINTS_CRATE 1000
#define NUMBER_ALIENS 5
#define NUMBER_BLACKHOLES 7

#define INVALID_TID ((pthread_t)(-1))

#ifndef SNP_TUTOR_TEST_1_H
#define SNP_TUTOR_TEST_1_H

typedef struct
{
  unsigned char x_;
  unsigned char y_;
} position;

typedef struct
{
  unsigned char pos_x_;
  unsigned char pos_y_;
  unsigned char type_;
} parameters;

extern WINDOW *game_window;
extern char game_map[MAP_LENGTH][MAP_WIDTH];

extern int lifepoints;
extern int points;
extern pthread_t enemy_alien_tid[NUMBER_ALIENS];
extern pthread_t enemy_blackhole_tid[NUMBER_BLACKHOLES];
extern position player_position;
extern pthread_t player_tid;
extern pthread_t crate_tid;
extern time_t time1;
extern int crate_collected_flag;

void init_map();

void spawnCrate(int x_pos, int y_pos);
void spawnEnemy(char type, int x_pos, int y_pos);
char getMapValue(int x_pos, int y_pos);

void refreshMap();

int end_game(void *rvalue, void **rvalues_enemies, void *rvalue_crate);

void *playerLogic();

void *enemyBlackhole(parameters *params);
void *enemyAlien(parameters *params);

void *placeCrate(parameters *params);

void init_enemies(unsigned char type, int number_of_enemy_type);

void movePlayer(char *direction, char *prev_direction, position *pos, char type);

char getRandPosX();

char getRandPosY();

#endif // SNP_TUTOR_TEST_1_H
