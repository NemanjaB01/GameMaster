
#ifndef A2_RESOURCES_H
#define A2_RESOURCES_H
#include "pthread.h"
#include "stdint.h"
#include "stdlib.h"
#include <semaphore.h>

/* STUDENT TODO:
 *  make sure shared resources are locked correctly
 *  add any needed variables here
 */

typedef enum{
  WASHING_OFF, 
  WASHING_ON,
  NEEDS_MAINTENANCE,
  CLOSED
} Mode;

typedef struct {
  pthread_t customer;
  ssize_t id;
} Customer;

typedef struct{
  pthread_t wash_bay;
  ssize_t id;

  pthread_cond_t washing_finished_cond;
  pthread_cond_t washing_program_selected;
  pthread_cond_t auto_clean_cond;
  pthread_mutex_t washing_bay_private_mutex;
  
  Mode mode;

  Customer* current_customer;
} WashBay;

typedef struct {
  pthread_t employee;
  ssize_t id;
} Employee;

#endif //A2_RESOURCES_H
