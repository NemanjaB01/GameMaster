#include "Player.h"

/*-----------------------------------------------------------------
 *  Main routine that loops and makes requests and eventually
 *  makes it public to the server. Stops when the operation equals
 *  zero. Initialize everything needed in here. All checks in this
 *  function must not be relocated.
 *
 *  @param argc number of arguments given
 *  @param argv char* that points to the argument
 *
 *  @return predefined value
 **/
int startPlayer()
{
  char buffer[REQUEST_MAX_LENGTH];
  initSharedMemoriesPlayer();
  checkSHMPlayer();
  initMmapingsPlayer();
  checkMMAPPlayer();
  initLocks();
  initProcess();
  checkGameMaster();

  do
  {
    // TODO Student START
    // process commands and values, synchronization,
    // assigning/receiving to/from shared memory,
    // break loop if no command is received or if command is exit.
    // (exit still has to be handled by the game master, though)

    // TODO Student END

    checkResults();

  } while (strcmp(buffer, CMD_EXIT) != 0);

  printf("[PLAYER] Bye!\n");

  closeMmapingsPlayer();
  checkCleanup();

  return 0;
}

/*-----------------------------------------------------------------
 *  This function starts and initializes the game master, the process does not
 *  run at this point. Be sure everything is defined and initialized.
 *  Use the predefined variables for the process id.
 *
 *  @param none
 *
 *  @return none
 **/
void initProcess()
{
  if (checkSetup())
  {
    return;
  }

  // TODO Student START
  // start the game master and load the right executable

  // TODO Student END
}

/*-----------------------------------------------------------------
 * initializes your shared objects on the Player and resizes them
 * make sure you only give the permissions the Player needs, use 
 * the provided defines (in util.h)
 * but please use MODERW, for testing purposes
 *
 * @param none
 *
 * @return none
 */
void initSharedMemoriesPlayer()
{
  // TODO Student START
  
  // TODO Student END
}

/*-----------------------------------------------------------------
 * maps the shared objects to the virtual memory space of the Player
 *
 * @param none
 *
 * @return none
 */
void initMmapingsPlayer()
{
  if (checkProgressPlayer())
  {
    return;
  }

  // TODO Student START
  
  // TODO Student END
}

/*-----------------------------------------------------------------
 * initializes the locks on the shared object
 *
 * @param none
 *
 * @return none
 */
void initLocks()
{
  // TODO Student START
  
  // TODO Student END
}
/*-----------------------------------------------------------------
 * removes all mappings and shared objects as seen from the Player
 * this part is an essential function for closing this application
 * accordingly without leaving artifacts on your system!
 *
 * @param none
 *
 * @return none
 */
void closeMmapingsPlayer()
{
  // TODO Student START
  
  // TODO Student END
}
