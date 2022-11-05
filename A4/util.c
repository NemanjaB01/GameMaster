#include "util.h"

/*-----------------------------------------------------------------
 * reads from stdin and processes the string entered for an easier
 * handling of commands.
 *
 * @param char pointer to the buffer that will be processed
 *
 * @return int true/false when successfully read from the stdin
 */
int getCommand(char *buffer)
{
  memset(buffer, 0, 30 * sizeof(char));
  if (fgets(buffer, 30 * sizeof(char), stdin) != NULL)
  {
    return 1;
  }
  else
    return 0;
}

/*-----------------------------------------------------------------
 * predefined function for testing. This is just a wrapper for testing
 * the specific results, you can output the response and the game
 * info here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkResults()
{
  printf("[GAMEMASTER] %s", mmaps.mapped_region_response_->message);
  printGameInfo();
}

/*-----------------------------------------------------------------
 * print game info. you can extend or modify the output as you like.
 *
 * @param none
 *
 * @return none
 */
void printGameInfo()
{
  if (mmaps.mapped_region_game_state_->game_active)
    printf("%s    (%d tries left)\n", mmaps.mapped_region_game_state_->puzzle,
           mmaps.mapped_region_game_state_->player_lifepoints);
}

/*-----------------------------------------------------------------
 * wrapper function for testing
 *thread.
 *
 * @param none
 *
 * @return none
 */
void checkGameMaster()
{
  if (process_id == -1)
    exit(-3);
}

/*-----------------------------------------------------------------
 * checks the read- and write capabilities of the shared objects
 * initialized before.
 * You can extend our provided tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkSHMrw(int fd)
{
  if (fd == -1)
  {
    exit(-1);
  }
}
/*-----------------------------------------------------------------
 * checks the read-only capabilities of the shared objects
 * initialized before.
 * You can extend our provided tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkSHMro(int fd)
{
  if (fd == -1)
  {
    exit(-1);
  }
}

/*-----------------------------------------------------------------
 * wrapper function for testing the shared objects
 *
 * @param none
 *
 * @return none
 */
void checkSHMPlayer()
{
  // use checkSHMro and checkSHMrw here if you want
}

void checkSHMGameMaster()
{
  // use checkSHMro and checkSHMrw here if you want
}

/*-----------------------------------------------------------------
 * predefined function for testing.
 * wrapper function for testing the mmap capabilities.
 * You can extend our provided tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkMMAPPlayer()
{
  // TODO optionally add some custom checks if needed
  if (mmaps.mapped_region_request_ == NULL || mmaps.mapped_region_game_state_ == NULL ||
      mmaps.mapped_region_locks_ == NULL || mmaps.mapped_region_response_ == NULL)
  {
    exit(-2);
  }
}

/*-----------------------------------------------------------------
 * predefined function for testing.
 * wrapper function for testing the mmap capabilities.
 * You can extend our provided tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkMMAPGameMaster()
{
  // TODO optionally add some custom checks if needed
  if (mmaps.mapped_region_request_ == NULL || mmaps.mapped_region_game_state_ == NULL ||
      mmaps.mapped_region_locks_ == NULL || mmaps.mapped_region_response_ == NULL)
  {
    exit(-2);
  }
}

/*-----------------------------------------------------------------
 * predefined function for testing. Checks if everything was cleaned
 * up properly in both processes. Add your own tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkCleanup()
{
}

/*-----------------------------------------------------------------
 * predefined function for testing.
 *
 * @param none
 *
 * @return none
 */
int checkSetup()
{
  return 0;
}

/*-----------------------------------------------------------------
 * predefined function for testing.
 *
 * @param none
 *
 * @return none
 */
int checkProgressGameMaster()
{
  return 0;
}

/*-----------------------------------------------------------------
 * predefined function for testing.
 *
 * @param none
 *
 * @return none
 */
int checkProgressPlayer()
{
  return 0;
}

/*-----------------------------------------------------------------
 * predefined function to choose next puzzle index.
 *
 * @param none
 *
 * @return none
 */
size_t selectRandomPuzzleIndex(size_t num_puzzles)
{
  return rand() % num_puzzles;
}

/*-----------------------------------------------------------------
 * programs main function. Here for testing reasons.
 *
 * @param none
 *
 * @return none
 */
int main()
{
#ifdef START
  return START();
#endif
}
