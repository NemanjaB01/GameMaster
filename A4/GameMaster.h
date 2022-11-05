#pragma once
#ifndef ASS4_GAMEMASTER_H
#define ASS4_GAMEMASTER_H

#include "util.h"

/*
 * Initialization of global variables
 */
filedescriptors fds = {-1, -1, -1, -1};
mappings mmaps = {NULL, NULL, NULL, NULL};
pid_t process_id = -1;

char *puzzles[] = {"mutex", "semaphore", "fun", "deadlock", "slp"};
char *active_puzzle = NULL;

/*
 * Various headers for used functions in server.c
 */
int startGameMaster();
void initSharedMemoriesGameMaster();
void initMmapingsGameMaster();
void closeMmapingsGameMaster();

char *cmdHandler(shmrequest *request);

int isCommandGuess(char *message);
int isCommandSolve(char *message);

void setUpNewPuzzle();
int updatePuzzle(char guess_char);
int checkPuzzleSolved(char *word);
void clearPuzzle();

#endif // ASS4_GAMEMASTER_H
