#include "GameMaster.h"

char *messages[] = {
    "I'm thinking of a word, guess it!\n",
    "The round has already started!\n",
    "The round has not yet started!\n",
    "Stopped round, maybe you want to try again?\n",
    "That was correct :)\n",
    "That was incorrect :(\n",
    "You won! Thank you for playing!\n",
    "You lost! Thank you for playing!\n",
    "I don't understand...\n",
    "Bye!\n"};

/*-----------------------------------------------------------------
 *  Main routine that loops and forwards requests and eventually
 *  makes it public to the Player. stops when "exit game" is received.
 *  Initialize everything needed in here.
 *
 *  @param argc number of arguments given
 *  @param argv char* that points to the argument
 *
 *  @return predefined value
 **/
int startGameMaster()
{
  initSharedMemoriesGameMaster();
  checkSHMGameMaster();
  initMmapingsGameMaster();
  checkMMAPGameMaster();

  printf("[GAMEMASTER] Welcome!\n");
  do
  {
    // TODO Student START
  
    sem_wait(&mmaps.mapped_region_locks_->request_semaphore);
    char* response = cmdHandler(mmaps.mapped_region_request_);
    strcpy(mmaps.mapped_region_response_->message, response);
    sem_post(&mmaps.mapped_region_locks_->response_semaphore);    
    // TODO Student END

  } while (strcmp(mmaps.mapped_region_request_->message, CMD_EXIT) != 0);

  closeMmapingsGameMaster();
  checkCleanup();

  return 0;
}

/*-----------------------------------------------------------------
 * initializes the GameMasters shared objects and resizes them, make 
 * sure to only give the permissions the GameMaster needs.
 * but please use MODERW, for testing purposes
 *
 * @param none
 *
 * @return none
 */
void initSharedMemoriesGameMaster()
{
  // TODO Student START
  fds.fd_shm_request_ = shm_open(SHM_NAME_REQUEST, FLAGS_SHM_READONLY, MODERW);

  fds.fd_shm_game_state_ = shm_open(SHM_NAME_GAMESTATE, FLAGS_SHM_READWRITE, MODERW);
  ftruncate(fds.fd_shm_game_state_, sizeof(shmgamestate));

  fds.fd_shm_response_ = shm_open(SHM_NAME_RESPONSE, FLAGS_SHM_READWRITE, MODERW);
  ftruncate(fds.fd_shm_response_, sizeof(shmresponse));

  fds.fd_shm_locks_ = shm_open(SHM_NAME_LOCKS, FLAGS_SHM_READWRITE, MODERW);
  // TODO Student END
}

/*-----------------------------------------------------------------
 * maps the shared objects to the virtual memory space of the GameMaster
 *
 * @param none
 *
 * @return none
 */
void initMmapingsGameMaster()
{
  if (checkProgressGameMaster())
  {
    return;
  }

  // TODO Student START
  mmaps.mapped_region_request_ = mmap(NULL, sizeof(shmrequest), PROT_READ, MAP_SHARED, fds.fd_shm_request_, 0);
  close(fds.fd_shm_request_);
  mmaps.mapped_region_game_state_ = mmap(NULL, sizeof(shmgamestate), PROT_READ | PROT_WRITE, MAP_SHARED,fds.fd_shm_game_state_, 0);
  close(fds.fd_shm_game_state_);
  mmaps.mapped_region_response_ = mmap(NULL, sizeof(shmresponse), PROT_READ | PROT_WRITE, MAP_SHARED,fds.fd_shm_response_, 0);
  close(fds.fd_shm_response_);
  mmaps.mapped_region_locks_ = mmap(NULL, sizeof(shmlocks) , PROT_READ | PROT_WRITE, MAP_SHARED, fds.fd_shm_locks_, 0);
  close(fds.fd_shm_locks_);  
  // TODO Student END
}

/*-----------------------------------------------------------------
 * removes all mappings and shared objects as seen from the game master
 * this part is an essential function for closing this application
 * accordingly without leaving artifacts on your system!
 *
 * @param none
 *
 * @return none
 */
void closeMmapingsGameMaster()
{
  if (checkProgressGameMaster())
  {
    return;
  }

  // TODO Student START
  munmap(mmaps.mapped_region_game_state_, sizeof(shmgamestate));
  munmap(mmaps.mapped_region_response_, sizeof(shmresponse));
  munmap(mmaps.mapped_region_locks_, sizeof(shmlocks));
  munmap(mmaps.mapped_region_request_, sizeof(shmrequest));  
  // TODO Student END
}

/*-----------------------------------------------------------------
 * handles the commands and responds with the appropriate message
 *
 * @param shmrequest the pointer to the request object
 *
 * @return char* the answer to the request
 */
char *cmdHandler(shmrequest *request)
{
  // TODO Student START
  // handle incoming commands, use provided functions:
  // setUpNewPuzzle(), updatePuzzle(), checkPuzzleSolved(), clearPuzzle()
  if(strcmp(request->message, CMD_EXIT) == 0)
  {
    clearPuzzle();
    mmaps.mapped_region_game_state_->game_active = 0;
    return messages[9];
  }
  else if(strcmp(request->message, CMD_START) == 0)
  {
    if(mmaps.mapped_region_game_state_->game_active == 1)
      return messages[1];
    
    mmaps.mapped_region_game_state_->game_active = 1;
    setUpNewPuzzle();
    mmaps.mapped_region_game_state_->player_lifepoints = PLAYER_INITIAL_LIFEPOINTS;
    return messages[0];
  }
  else if(isCommandGuess(&request->message[0]) == 1)
  {
    if(mmaps.mapped_region_game_state_->game_active == 0)
      return messages[2];
    if(updatePuzzle(request->message[6]) == 0)
    {
      mmaps.mapped_region_game_state_->player_lifepoints--;
      if(mmaps.mapped_region_game_state_->player_lifepoints == 0)
      {
        mmaps.mapped_region_game_state_->game_active = 0;
        clearPuzzle();
        return messages[7];
      }
      else
       return messages[5];
    }

    for(int pos = 0; pos < strlen(mmaps.mapped_region_game_state_->puzzle); pos++)
    {
      if(mmaps.mapped_region_game_state_->puzzle[pos] == 95)
        return messages[4];
    }
    
    mmaps.mapped_region_game_state_->game_active = 0;
    clearPuzzle();
    return messages[6];
  }
  if(isCommandSolve(request->message) == 1)
  {
    if(mmaps.mapped_region_game_state_->game_active == 0)
      return messages[2];
    else if(checkPuzzleSolved(&request->message[6]) == 1)
    {
      clearPuzzle();
      mmaps.mapped_region_game_state_->game_active = 0;
      return messages[6];
    }
    else
     return messages[7];;
  }

  if(strcmp(request->message, CMD_QUIT) == 0)
  {
    if(mmaps.mapped_region_game_state_->game_active == 0)
      return messages[2];
    mmaps.mapped_region_game_state_->game_active = 0;
    clearPuzzle();
    return messages[3];
  }


  // TODO Student END
  return messages[8];
}

/*-----------------------------------------------------------------
 * check if command is a guess command (keyword guess + space + 1 character)
 *
 * @param none
 *
 * @return 1 if true, 0 if not
 */
int isCommandGuess(char *message)
{
  if ((strncmp(message, CMD_GUESS, strlen(CMD_GUESS)) == 0) &&
      (strlen(message) == (strlen(CMD_GUESS) + 2)))
    return 1;
  else
    return 0;
}

/*-----------------------------------------------------------------
 * check if command is a solve command (keyword guess + space + at least
 * 1 character)
 *
 * @param none
 *
 * @return 1 if true, 0 if not
 */
int isCommandSolve(char *message)
{
  if ((strncmp(message, CMD_SOLVE, strlen(CMD_SOLVE)) == 0) &&
      (strlen(message) >= (strlen(CMD_SOLVE) + 2)))
    return 1;
  else
    return 0;
}

/*-----------------------------------------------------------------
 * choose a random word from list, set up the puzzle and set
 * active puzzle pointer
 *
 * @param none
 *
 * @return none
 */
void setUpNewPuzzle()
{
  size_t index = selectRandomPuzzleIndex(sizeof(puzzles)/sizeof(char *));
  active_puzzle = puzzles[index];

  memset(mmaps.mapped_region_game_state_->puzzle, '_', strlen(active_puzzle));

  char first_char = active_puzzle[0];
  updatePuzzle(first_char);
}

/*-----------------------------------------------------------------
 * update the puzzle with guessed character, if guess was correct
 *
 * @param guess_char guessed character
 *
 * @return 1 if guess correct, 0 if not
 */
int updatePuzzle(char guess_char)
{
  // handle upper case letters
  if (guess_char >= 'A' && guess_char <= 'Z')
    guess_char += 32;

  // update puzzle if guess correct
  int guess_correct = 0;
  for (int pos = 0; pos < strlen(active_puzzle); pos++)
  {
    if (active_puzzle[pos] == guess_char)
    {
      memset(&mmaps.mapped_region_game_state_->puzzle[pos], guess_char, 1);
      guess_correct = 1;
    }
  }
  return guess_correct;
}

/*-----------------------------------------------------------------
 * check if puzzle is already solved
 * can be used to check the game state puzzle as well as user
 * input via the solve command
 *
 * @param char * to string to check using active_puzzle as reference
 *
 * @return 1 if solved, 0 if not
 */
int checkPuzzleSolved(char *word)
{
  if ((strncmp(active_puzzle, word, strlen(active_puzzle)) == 0) &&
      (strlen(word) == strlen(active_puzzle) ||
      (strlen(word) == strlen(active_puzzle)+1)))  // allow the additional '\n' in command
    return 1;

  return 0;
}

/*-----------------------------------------------------------------
 * clear the puzzle and reset active_puzzle
 *
 * @param none
 *
 * @return none
 */
void clearPuzzle()
{
  active_puzzle = NULL;
  memset(mmaps.mapped_region_game_state_->puzzle, 0, WORD_MAX_LENGTH);
}