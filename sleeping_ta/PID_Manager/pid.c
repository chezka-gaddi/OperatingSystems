/**
 * A solution to the pid manager problem. 
 *
 */

#include "pid.h"
#include <pthread.h>
#include <stdio.h>

// Allocates the pid map.
int allocate_map(void) 
{
  last = PID_MIN;
	
  // Successful allocation of map
  if (memset(pid_map, 0, PID_MAX+1))
  {
    // Initialize mutex for the pid array
    if(pthread_mutex_init(&mutex, NULL) != 0)
    {
      printf("\n Mutex init failed\n");
      return -1;
    }
    
    return 0;
  }
  // Unsuccessful allocation
  return -1;
}

// Allocates a pid
int allocate_pid(void)
{
    /* acquire the mutex lock and warn if unable */
    if(pthread_mutex_lock(&mutex) == -1)
        printf("Unable to acquire mutex lock.");
  
  
    /* find the next available pid and reset count to PID_MIN */
    while(pid_map[last] != 0)
    {
        last += 1;
    
        if(last >= PID_MAX)
            last = PID_MIN;
    }
 
    /* indicate that pid is in use */
    pid_map[last] = 1;
    printf("allocated %d.\n", last);
  
    /* release and warn if the mutex was not released */
    if (pthread_mutex_unlock(&mutex) == -1)
    {
        printf("Mutex was not released.");
        return -1;
    }
  
    return last;
}

// Releases a pid
void release_pid(int pid)
{
    /* acquire the mutex lock and warn if unable */
    if(pthread_mutex_lock(&mutex) == -1)
        printf("Unable to acquire mutex lock.");

    /* indicate that pid is not availble to use again */
    pid_map[pid] = 0;

    /* release and warn if the mutex was not released  */
    if (pthread_mutex_unlock(&mutex) == -1)
        printf("Mutex was not released.");

    printf("released %d.\n", pid);
}
