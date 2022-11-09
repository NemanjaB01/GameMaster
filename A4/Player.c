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
    // break the loop if the command received is 'exit game'.
    // (exit still has to be handled by the game master, though)
  
    int rvalue = getCommand(&buffer[0]);
    if(rvalue == 0)
      exit(-1);
    strcpy(mmaps.mapped_region_request_->message, &buffer[0]);
    sem_post(&mmaps.mapped_region_locks_->request_sem);

    sem_wait(&mmaps.mapped_region_locks_->response_sem);
    // TODO Student END

    checkResults();

  } while (strcmp(buffer, CMD_EXIT) != 0);
  wait(NULL);
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
  process_id = fork();
  if(process_id == 0)
  {
    char *argv[]= {"./GameMaster", NULL};
    execv(argv[0],argv);
  }


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
  
  fds.fd_shm_request_ = shm_open(SHM_NAME_REQUEST, FLAGS_SHM_READWRITE, MODERW);
  ftruncate(fds.fd_shm_request_, sizeof(shmrequest));

  fds.fd_shm_game_state_ = shm_open(SHM_NAME_GAMESTATE,  FLAGS_SHM_READONLY, MODERW);

  fds.fd_shm_response_ = shm_open(SHM_NAME_RESPONSE, FLAGS_SHM_READONLY, MODERW);

  fds.fd_shm_locks_ = shm_open(SHM_NAME_LOCKS, FLAGS_SHM_READWRITE, MODERW);
  ftruncate(fds.fd_shm_locks_, sizeof(shmlocks));
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
  mmaps.mapped_region_request_ = mmap(NULL, sizeof(shmrequest), PROT_READ | PROT_WRITE, MAP_SHARED, fds.fd_shm_request_, 0);
  close(fds.fd_shm_request_);
  mmaps.mapped_region_game_state_ = mmap(NULL, sizeof(shmgamestate), PROT_READ, MAP_SHARED,fds.fd_shm_game_state_, 0);
  close(fds.fd_shm_game_state_);
  mmaps.mapped_region_response_ = mmap(NULL, sizeof(shmresponse), PROT_READ , MAP_SHARED,fds.fd_shm_response_, 0);
  close(fds.fd_shm_response_);
  mmaps.mapped_region_locks_ = mmap(NULL, sizeof(shmlocks), PROT_READ | PROT_WRITE, MAP_SHARED, fds.fd_shm_locks_, 0);
  close(fds.fd_shm_locks_);
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
  sem_init(&mmaps.mapped_region_locks_->request_sem, 1, 0);
  sem_init(&mmaps.mapped_region_locks_->response_sem, 1, 0);
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
  sem_destroy(&mmaps.mapped_region_locks_->request_sem);
  sem_destroy(&mmaps.mapped_region_locks_->response_sem);
  munmap(mmaps.mapped_region_locks_, sizeof(shmlocks));
  munmap(mmaps.mapped_region_request_, sizeof(shmrequest));
  munmap(mmaps.mapped_region_game_state_, sizeof(shmgamestate));
  munmap(mmaps.mapped_region_response_, sizeof(shmresponse));
  shm_unlink(SHM_NAME_REQUEST);
  shm_unlink(SHM_NAME_LOCKS);
  
  // TODO Student END
}
