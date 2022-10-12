
#ifndef A2_RESOURCES_H
#define A2_RESOURCES_H
#include "pthread.h"
#include "stdint.h"
#include "stdlib.h"

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

  Mode mode;

  Customer* current_customer;
} WashBay;

typedef struct {
  pthread_t employee;
  ssize_t id;
} Employee;

#endif //A2_RESOURCES_H
