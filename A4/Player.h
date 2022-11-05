#pragma once
#ifndef ASS4_PLAYER_H
#define ASS4_PLAYER_H

#include "util.h"

/*
 * Initialization of global variables
 */
filedescriptors fds = {-1, -1, -1, -1};
mappings mmaps = {NULL, NULL, NULL, NULL};
pid_t process_id = -1;

/*
 * Various headers for used functions in Player.c
 */
int startPlayer();
void initProcess();
void initSharedMemoriesPlayer();
void initMmapingsPlayer();
void initLocks();
int getCommand(char *buffer);
void closeMmapingsPlayer();

#endif // ASS4_PLAYER_H
