/*
 * DO NOT TOUCH THIS FILE!
 */
#ifndef A2_DEFINITIONS_H
#define A2_DEFINITIONS_H
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <string.h>

#define UPPER_LIMIT_WORKING_STUFF 100
#define UPPER_LIMIT_CUSTOMERS 2000
#define UPPER_LIMIT_WASH_BAYS 1000
#define LOWER_LIMIT              0

#define NUM_WORKING_STAFF "num_employees"
#define NUM_CUSTOMERS "num_customers"
#define NUM_WASH_BAYS "num_wash_bays"

#define NUM_VACUUM_STATIONS 2

void checkAllowedRange(ssize_t value, ssize_t upper_limit, char* name)
{
  if (value < LOWER_LIMIT || value > upper_limit) {
    fprintf(stderr, "%s allowed range: [%d,%zd].\n", name, LOWER_LIMIT, upper_limit);
    exit(-1);
  }
}

void checkParametersValidity(ssize_t num_working_staff, ssize_t num_customers, ssize_t num_wash_bays) 
{
  checkAllowedRange(num_working_staff, UPPER_LIMIT_WORKING_STUFF, NUM_WORKING_STAFF);
  checkAllowedRange(num_customers, UPPER_LIMIT_CUSTOMERS, NUM_CUSTOMERS);
  checkAllowedRange(num_wash_bays, UPPER_LIMIT_WASH_BAYS, NUM_WASH_BAYS);
}

void coffeeTime() 
{
  ssize_t microsec_to_sleep = 1000 + rand() % (20 * 1000);
  nanosleep((const struct timespec[]){{0, 1000L * microsec_to_sleep}}, NULL);
}

void automizedCleaning() 
{
  ssize_t microsec_to_sleep = 1000 + rand() % (7 * 1000);
  nanosleep((const struct timespec[]){{0, 1000L * microsec_to_sleep}}, NULL);
}

void carVacuuming() 
{
  ssize_t microsec_to_sleep = 1000 + rand() % (15 * 1000);
  nanosleep((const struct timespec[]){{0, 1000L * microsec_to_sleep}}, NULL);
}

void washTheCar()
{
  ssize_t microsec_to_sleep = 1000 + rand() % (15 * 1000);
  nanosleep((const struct timespec[]){{0, 1000L * microsec_to_sleep}}, NULL);  
}


void washBayMaintenance()
{
  ssize_t microsec_to_sleep = 1000 + rand() % (13 * 1000);
  nanosleep((const struct timespec[]){{0, 1000L * microsec_to_sleep}}, NULL);  
}

void driveIn()
{
  ssize_t microsec_to_sleep = 1000 + rand() % (6 * 1000);
  nanosleep((const struct timespec[]){{0, 1000L * microsec_to_sleep}}, NULL);  
}

ssize_t isMaintenanceNeeded() 
{
  return rand() % 2;
}

#endif //A2_DEFINITIONS_H
