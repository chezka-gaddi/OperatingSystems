#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include "ta.h"

pthread_t ta;
pthread_t students[NUM_OF_STUDENTS];

/**
 * Initialize all relevant data structures and
 * synchronization objects.
 */
void init()
{
    /* initialize variables */
    int i;
    curr_student = 0;
    next_seat = 0;

    /* initialize mutex lock */
    if (pthread_mutex_init(&mutex, NULL) != 0)
        printf("%s\n",strerror(errno));

    /* initialize semaphore for student waiting */
    if (sem_init(&sem_students, 0, 0) != 0)
        printf("error init sem_students\n"); 
	
    /* initialize semaphore for ta */
    if (sem_init(&sem_ta, 0, 1) != 0)
        printf("error init sem_ta\n");

    /* populate student IDs */
    for (i = 0; i < NUM_OF_STUDENTS; i++)
        studentIDs[i] = i;
}

/**
 * Create the student threads and assign them the 
 * student_loop function and their id number.
 */
void create_students()
{
    int i;
    for (i = 0; i < NUM_OF_STUDENTS; i++) { 
        pthread_create(&(students[i]), NULL, &student_loop, (void*) &studentIDs[i]); 
    }
}

/**
 * Create the ta thread and assign it the ta_loop 
 * function.
 */
void create_ta()
{
    pthread_create(&ta, NULL, &ta_loop, NULL);
}

/**
 * Initializes all structures and variables and creates the student threads and ta thread. Joins all student threads and then cancel the TA thread.
 */
int main(void)
{
    int i;

    init();
    create_ta();
    create_students();

    /* synch all threads */
    for (i = 0; i < NUM_OF_STUDENTS; i++)
        pthread_join(students[i], NULL);
    
    /* when all students have finished, we will cancel the TA thread */	
    if (pthread_cancel(ta) != 0)
        printf("%s\n",strerror(errno));

    /* destroy mutexes and semaphores used */
    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem_students);
    sem_destroy(&sem_ta);

    return 0;
}
