#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>

#include "definitions.h"
#include "resources.h"
#include "vector.h"
/*
 * HINTS:
    ~  Make sure EVERY shared resource is locked properly!
    ~  Consider establishing a valid customer - automatic wash machine sync first
    ~  Be aware of 'lost' signals!
    ~
 */

int vacuum_stations[NUM_VACUUM_STATIONS];
vector free_wash_bays;
int count_washed_cars = 0;
bool carwash_opened = false;

/*
 * STUDENT TODO BEGIN:
 *  locking-related global variables may be declared here
 */
 sem_t vacuum_station_semaphore;
 sem_t available_wash_bays;
 pthread_cond_t opening_carwash_cond;
 pthread_mutex_t wash_bay_mutex;
 pthread_mutex_t vacuum_mutex;
 pthread_mutex_t counting_mutex;
 pthread_mutex_t car_wash_opened_mutex;

/*
 * STUDENT TODO END:
 */

// ------------------------------------------------------------------------
/* STUDENT TODO:
 *  make sure : 
    ~ shared resources are locked correctly
    ~ every customer gets to the vacuum station
 */
void goToTheVacuumStation(Customer* customer) 
{
  sem_wait(&vacuum_station_semaphore);
  printf("Customer %zd goes to the vacuum station...\n", customer->id);

  int i = 0;
  
  pthread_mutex_lock(&vacuum_mutex);
  while (vacuum_stations[i] != 0) 
  {
    i++;
  }
  vacuum_stations[i] = 1;
  
  printf("Customer %zd vacuums his car...\n", customer->id);
  pthread_mutex_unlock(&vacuum_mutex);

  carVacuuming();
  pthread_mutex_lock(&vacuum_mutex);
  vacuum_stations[i] = 0;
  pthread_mutex_unlock(&vacuum_mutex);

  printf("Customer %zd left vacuum station...\n", customer->id);  
  sem_post(&vacuum_station_semaphore);

}

// ------------------------------------------------------------------------
/* STUDENT TODO:
 *  make sure : 
    ~ shared resources are locked correctly
    ~ customers start looking for a free bay after opening
    ~ the customer gets a free, working wash bay
    ~ be aware of "lost signals"
 */
WashBay* driveIntoTheWashBay(Customer* customer) 
{
  pthread_mutex_lock(&car_wash_opened_mutex);
  while(carwash_opened == false)
  {
    pthread_cond_wait(&opening_carwash_cond, &car_wash_opened_mutex);
  }
  pthread_mutex_unlock(&car_wash_opened_mutex);
  
  sem_wait(&available_wash_bays);
  printf("Customer %zd is looking for a free wash bay.\n", customer->id);
  pthread_mutex_lock(&wash_bay_mutex);
  WashBay* free_wash_bay = 0;
  
  // find a free car wash bay
  vector_iterator it = vector_begin(&free_wash_bays);
  vector_iterator it_end = vector_end(&free_wash_bays);
  vector_iterator wash_bay_it;

  for (; it != it_end; ++it) 
  {
    WashBay* tmp = *it;
    if (tmp->mode != NEEDS_MAINTENANCE)  
    {
      wash_bay_it = it;
      break;
    }
  }

  driveIn();

  free_wash_bay = *wash_bay_it;
  
  if (free_wash_bay) 
  {
    vector_erase(&free_wash_bays, wash_bay_it);
    pthread_mutex_unlock(&wash_bay_mutex);
    printf("Customer %zd found a free wash bay %zd\n", customer->id, free_wash_bay->id);
  }
  
  if(free_wash_bay && free_wash_bay->current_customer)
  {
    printf("Customer %zd tries to drive into a used wash bay :o\n", customer->id);
  }

  pthread_mutex_lock(&free_wash_bay->washing_bay_private_mutex);
  free_wash_bay->current_customer = customer;
  pthread_mutex_unlock(&free_wash_bay->washing_bay_private_mutex);

  return free_wash_bay;
}

// ------------------------------------------------------------------------
/* STUDENT TODO:
 *  make sure : 
    ~ shared resources are locked correctly
    ~ the customer picks up the car at the end of the program
    ~ auto cleaning is triggered upon leaving the wash bay
 */
void leaveTheWashBay(Customer* customer, WashBay* wash_bay) 
{
  sem_wait(&wash_bay->washing_done);
  pthread_mutex_lock(&wash_bay->washing_bay_private_mutex);
  if(wash_bay->current_customer != customer)
  {
    printf("Customer %zd didn't find his car in the wash bay :o.\n", customer->id);
  }

  if(isMaintenanceNeeded())
  {
    wash_bay->mode = NEEDS_MAINTENANCE;
  }
  else
  {
    wash_bay->mode = WASHING_OFF;
  }
  
  printf("Customer %zd leaves the wash bay...\n", customer->id);
  pthread_mutex_unlock(&wash_bay->washing_bay_private_mutex);
  sem_post(&wash_bay->cleaning_begin);
}

// ------------------------------------------------------------------------
/* STUDENT TODO:
 *  make sure :
    ~ shared resources are locked correctly
    ~ the program selection starts the washing process
 */
void selectAWashingProgram(WashBay* wash_bay, Customer* customer)
{
  pthread_mutex_lock(&wash_bay->washing_bay_private_mutex);
  wash_bay->mode = WASHING_ON;
  printf("Customer %zd selected a washing program and can have a coffee break now.\n", customer->id);
  pthread_mutex_unlock(&wash_bay->washing_bay_private_mutex);
  pthread_cond_signal(&wash_bay->washing_program_selected);
}

// ------------------------------------------------------------------------
void* takeABreakAtCarWashPark(Customer* customer) 
{
  WashBay* wash_bay = driveIntoTheWashBay(customer);
  selectAWashingProgram(wash_bay, customer);
  coffeeTime();
  leaveTheWashBay(customer, wash_bay);
  goToTheVacuumStation(customer);
  
  return NULL;
}

// ------------------------------------------------------------------------
/* STUDENT TODO:
 *  make sure shared resources are locked correctly
 */
void maintainingWashBay(WashBay* wash_bay) 
{
  washBayMaintenance();
  pthread_mutex_lock(&wash_bay->washing_bay_private_mutex);
  wash_bay->mode = WASHING_OFF;
  pthread_mutex_unlock(&wash_bay->washing_bay_private_mutex);
}

// ------------------------------------------------------------------------
/* STUDENT TODO:
 *  make sure 
    ~ shared resources are locked correctly
    ~ employees don't start working before car wash park is opened
 */
void* checkWashBays(Employee* employee) 
{
  pthread_mutex_lock(&car_wash_opened_mutex);
  while(carwash_opened == false)
  {
    pthread_cond_wait(&opening_carwash_cond, &car_wash_opened_mutex);
  }
  pthread_mutex_unlock(&car_wash_opened_mutex);


  while (1) 
  {
    printf("Employee %zd is checking wash bays...\n", employee->id);
    pthread_mutex_lock(&wash_bay_mutex);
    WashBay* wash_bay = 0;
    vector_iterator it = vector_begin(&free_wash_bays);

    for (; it != vector_end(&free_wash_bays); ++it) 
    {
      WashBay* tmp = *it;
      if (tmp->mode == NEEDS_MAINTENANCE) 
      {
        wash_bay = *it;
        vector_erase(&free_wash_bays, it);
        break;
      }
    }
    pthread_mutex_unlock(&wash_bay_mutex);
    if (!wash_bay) 
    {
      printf("Employee %zd has nothing to do...\n", employee->id);
      coffeeTime();
      continue;
    }
    maintainingWashBay(wash_bay);
    pthread_mutex_lock(&wash_bay_mutex);
    vector_push_back(&free_wash_bays, wash_bay);
    pthread_mutex_unlock(&wash_bay_mutex);
    sem_post(&available_wash_bays);
  }
}

// ------------------------------------------------------------------------
/* STUDENT TODO:
 *  make sure :
    ~ shared resources are locked correctly
    ~ automatic wash machines are not active if the car wash park is closed
    ~ washing process doesn't start before program selection <-- mode correspondingly changed
    ~ the customer is notified as soon as the washing process is finished
    ~ auto cleaning activated afterward
 */
void automaticWashing(WashBay* wash_bay) 
{
  pthread_mutex_lock(&car_wash_opened_mutex);
  while(carwash_opened == false)
  {
    pthread_cond_wait(&opening_carwash_cond, &car_wash_opened_mutex);
  }
  pthread_mutex_unlock(&car_wash_opened_mutex);
  while(1)
  {
    printf("WashBay %zd : waiting for new customers.\n", wash_bay->id);
    
    pthread_mutex_lock(&wash_bay->washing_bay_private_mutex);
    while((wash_bay->mode != WASHING_ON) && (wash_bay->mode != CLOSED))
    {
      pthread_cond_wait(&wash_bay->washing_program_selected , &wash_bay->washing_bay_private_mutex);
    }
    if(wash_bay->mode == CLOSED)
    {
      pthread_mutex_unlock(&wash_bay->washing_bay_private_mutex);
      break;
    }

    if(wash_bay->mode != WASHING_ON)
    {
      printf("WashBay %zd : washing before wash program selected.\n", wash_bay->id); 
    }
    printf("WashBay %zd : washing car of customer %zd.\n", 
            wash_bay->id, wash_bay->current_customer->id);
    washTheCar(); 

    printf("WashBay %zd : the car of customer %zd washed, can be picked up.\n", 
            wash_bay->id, wash_bay->current_customer->id);
    pthread_mutex_unlock(&wash_bay->washing_bay_private_mutex);
    sem_post(&wash_bay->washing_done);


    sem_wait(&wash_bay->cleaning_begin);
    pthread_mutex_lock(&wash_bay->washing_bay_private_mutex);
    wash_bay->current_customer = 0;
    pthread_mutex_unlock(&wash_bay->washing_bay_private_mutex);
    automizedCleaning();

    pthread_mutex_lock(&wash_bay_mutex);
    vector_push_back(&free_wash_bays, wash_bay);
    pthread_mutex_unlock(&wash_bay_mutex);
  
    pthread_mutex_lock(&wash_bay->washing_bay_private_mutex);
    if(wash_bay->mode != NEEDS_MAINTENANCE)
    {
      printf("WashBay %zd is ready for new customers.\n", wash_bay->id);
      sem_post(&available_wash_bays);
    }
    pthread_mutex_unlock(&wash_bay->washing_bay_private_mutex);
  
    pthread_mutex_lock(&counting_mutex);
    count_washed_cars++;
    pthread_mutex_unlock(&counting_mutex);
  }
} 

// ------------------------------------------------------------------------
/* STUDENT TODO:
 *  make sure : 
    ~ shared resources are locked correctly
    ~ everyone is notified about the opening
 */
void openCarWashPark()
{
  pthread_mutex_lock(&car_wash_opened_mutex);
  carwash_opened = true;
  printf("CAR WASH PARK OPENED!\n");
  pthread_mutex_unlock(&car_wash_opened_mutex);
  pthread_cond_broadcast(&opening_carwash_cond);
}

// ------------------------------------------------------------------------
/* STUDENT TODO:
 *  make sure :
    ~ shared resources are locked correctly
    ~ mutex/cond/sem initialized and destroyed accordingly
 */
int main(int argc, char* argv[]) 
{
  srand(time(NULL));

  if (argc != 4) 
  {
    fprintf(stderr, "Usage: %s <num_employees> <num_customers> <num_wash_bays>\n", argv[0]);
    exit(-1);
  }

  // check the validity of parameters
  ssize_t num_working_staff = atoi(argv[1]);
  ssize_t num_customers = atoi(argv[2]);
  ssize_t num_wash_bays = atoi(argv[3]);

  checkParametersValidity(num_working_staff, num_customers, num_wash_bays);
  
  Employee* employees = malloc(num_working_staff * sizeof(Employee));
  Customer* customers = malloc(num_customers * sizeof(Customer));
  
  if (!employees || !customers) 
  {
    free(employees);
    free(customers);
    fprintf(stderr, "Could not allocate memory!\n");
    exit(-1);
  }

  sem_init(&vacuum_station_semaphore, 0, NUM_VACUUM_STATIONS);
  sem_init(&available_wash_bays, 0 , num_wash_bays);
  pthread_mutex_init(&wash_bay_mutex, 0);
  pthread_mutex_init(&vacuum_mutex, 0);
  pthread_mutex_init(&counting_mutex, 0);
  pthread_mutex_init(&car_wash_opened_mutex, 0);
  pthread_cond_init(&opening_carwash_cond, 0);
  
  // handle free_wash_bays vector
  vector_init(&free_wash_bays);
  WashBay* wash_bays[num_wash_bays];

  for (ssize_t i = 0; i < num_wash_bays; i++) 
  {
    WashBay* wash_bay = malloc(sizeof(WashBay));

    if(!wash_bay)
    {
      free(wash_bay);
      fprintf(stderr, "Could not allocate memory!\n");
      exit(-1);
    }
    pthread_cond_init(&wash_bay->washing_program_selected, 0);
    pthread_mutex_init(&wash_bay->washing_bay_private_mutex, 0);
    sem_init(&wash_bay->washing_done, 0, 0);
    sem_init(&wash_bay->cleaning_begin, 0 , 0);

    wash_bay->mode = WASHING_OFF;
    wash_bay->id = i;
    wash_bay->current_customer = 0;
    vector_push_back(&free_wash_bays, wash_bay);
    
    // create car wash machine threads
    wash_bays[i] = wash_bay;
    assert(!pthread_create(&wash_bay->wash_bay, NULL, 
                           (void*(*)(void*))automaticWashing, (void*)wash_bay));   
    
  }

  // create employee threads
  for (ssize_t i = 0; i < num_working_staff; i++) 
  {
    employees[i].id = i;
    assert(!pthread_create(&employees[i].employee, NULL, 
                      (void*(*)(void*))checkWashBays, (void*)&employees[i]));
  }

  // create customer threads
  for (ssize_t i = 0; i < num_customers; i++) 
  {
    customers[i].id = i;

    assert(!pthread_create(&customers[i].customer, NULL, 
                  (void*(*)(void*))takeABreakAtCarWashPark, (void*)&customers[i]));
  }
    
  // create a mystic employee to open the car wash park
  pthread_t mystic_employee;
  assert(!pthread_create(&mystic_employee, NULL, 
              (void*(*)(void*))&openCarWashPark, NULL));
              
  
  for (ssize_t i = 0; i < num_customers; i++) 
  {
    pthread_join(customers[i].customer, NULL);
  }

  for (ssize_t i = 0; i < num_working_staff; i++) 
  {
    pthread_cancel(employees[i].employee);
    pthread_join(employees[i].employee, NULL);
  }
  
  for(size_t i = 0; i < num_wash_bays; ++i)
  {
    pthread_mutex_lock(&wash_bays[i]->washing_bay_private_mutex);
    wash_bays[i]->mode = CLOSED;
    pthread_mutex_unlock(&wash_bays[i]->washing_bay_private_mutex);
    pthread_cond_signal(&wash_bays[i]->washing_program_selected);
    pthread_join(wash_bays[i]->wash_bay, NULL);
    pthread_cond_destroy(&wash_bays[i]->washing_program_selected);
    pthread_mutex_destroy(&wash_bays[i]->washing_bay_private_mutex);
    sem_destroy(&wash_bays[i]->washing_done);
    sem_destroy(&wash_bays[i]->cleaning_begin);

  }

  pthread_join(mystic_employee, NULL);

  printf("CAR WASH PARK CLOSED!\n");
  free(employees);
  free(customers);

  vector_iterator it = vector_begin(&free_wash_bays);
  while (it != vector_end(&free_wash_bays)) 
  {
    free(*it);
    vector_erase(&free_wash_bays, it);
  }
  vector_destroy(&free_wash_bays);
  sem_destroy(&vacuum_station_semaphore);
  sem_destroy(&available_wash_bays);
  pthread_cond_destroy(&opening_carwash_cond);
  pthread_mutex_destroy(&wash_bay_mutex);
  pthread_mutex_destroy(&vacuum_mutex);
  pthread_mutex_destroy(&counting_mutex);
  pthread_mutex_destroy(&car_wash_opened_mutex);

  return 0;
}
