#ifndef ASS4_UTIL_H
#define ASS4_UTIL_H

#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <semaphore.h>
#include <linux/filter.h>
#include <linux/audit.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

// Number of numbers that have to be read
#define EXT_TESTS 1

/*
 * Use those defines for the name required
 * to open shared objects
 */
#define SHM_NAME_REQUEST "request"
#define SHM_NAME_RESPONSE "response"
#define SHM_NAME_LOCKS "locks"
#define SHM_NAME_GAMESTATE "gamestate"

/*
 * Use those defines for checking the commands
 */
#define CMD_EXIT "exit game\n"
#define CMD_START "start round\n"
#define CMD_QUIT "quit round\n"
#define CMD_GUESS "guess "
#define CMD_SOLVE "solve "

/*
 * Other game defines. Use whenever possible.
 */
#define WORD_MAX_LENGTH 32
#define RESPONSE_MAX_LENGTH 45
#define REQUEST_MAX_LENGTH 30

#define PLAYER_INITIAL_LIFEPOINTS 5

/*
 * Can be used for testing
 */
#define TEST_STR "DEAD"
#define LENGTH_TEST_STR 0x4

/*
 * Important and useful predefined values
 * for shared objects
 */
#define FLAGS_SHM_READONLY O_RDONLY | O_CREAT
#define FLAGS_SHM_READWRITE O_RDWR | O_CREAT
#define MODERW S_IRUSR | S_IWUSR

/*
 *  Error codes you can use for testing.
 */
#define ERROR_SHM_PLAYER -1
#define ERROR_MMAP_PLAYER -3
#define ERROR_RES_PLAYER_F -5
#define ERROR_RES_PLAYER_P -6
#define ERROR_RES_PLAYER_I -7
#define ERROR_UNMAP_CLOSE_PLAYER -8


#define ERROR_SHM_GAMEMASTER -2
#define ERROR_MMAP_GAMEMASTER -4
#define ERROR_REQ_GAMEMASTER_F -5
#define ERROR_REQ_GAMEMASTER_P -6
#define ERROR_REQ_GAMEMASTER_I -7
#define ERROR_UNMAP_CLOSE_GAMEMASTER -8

/*
 * file descriptors of your shared objects
 */
typedef struct
{
  int fd_shm_request_;
  int fd_shm_game_state_;
  int fd_shm_response_;
  int fd_shm_locks_;
} filedescriptors;

/*
 * shared objects for your locks / semaphores / cond variables (you don't necessarily need them all)
 */
typedef struct
{
  // TODO Student START
  sem_t request_semaphore;
  sem_t response_semaphore;
  // TODO Student END
} shmlocks;

/*
 * the gamestate struct
 * can be extended if wanted
 */
typedef struct
{
  char puzzle[WORD_MAX_LENGTH];
  int player_lifepoints;
  int game_active;
} shmgamestate;

/*
 * the response struct
 * can be extended if wanted
 */
typedef struct
{
  char message[RESPONSE_MAX_LENGTH];
} shmresponse;

/*
 * the request struct
 * can be extended if wanted
 */
typedef struct
{
  char message[REQUEST_MAX_LENGTH];
} shmrequest;

/*
 *the according address pointing to the mapped region
 */
typedef struct
{
  shmrequest *mapped_region_request_;
  shmgamestate *mapped_region_game_state_;
  shmresponse *mapped_region_response_;
  shmlocks *mapped_region_locks_;
} mappings;

/*
 * global variables.You MUST USE them!!!!
 */
extern filedescriptors fds;
extern pid_t process_id;
extern mappings mmaps;

/*
 * check functions you should not move in the code (you can implement your own functionality in util.c)
 */
void removeAllSHM();
void checkResults();
int checkSetup();
void checkMMAPGameMaster();
void checkSHMGameMaster();
void checkMMAPPlayer();
void checkSHMPlayer();
void checkGameMaster();
void checkCleanup();

void printGameInfo();
int checkProgressPlayer();     
int checkProgressGameMaster();

size_t selectRandomPuzzleIndex(size_t num_puzzles);

extern int startPlayer();
extern int startGameMaster();

/*
 * Useful functions to use
 */
int getCommand(char *buffer);

#endif // ASS4_UTIL_H
