/**
 * Header file for sleeping TA
 */

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>


/* the maximum time (in seconds) to sleep */
#define MAX_SLEEP_TIME         5

/* number of maximum waiting students */
#define MAX_WAITING_STUDENTS   3

/* number of potential students */
#define NUM_OF_STUDENTS        5

/* number of available seats */
#define NUM_OF_SEATS           3

/* semaphores and mutex lock */
pthread_mutex_t mutex;

/* semaphore declarations */
sem_t sem_students;
sem_t sem_ta;

/* the number of waiting students */
int num_waiting;

/* chairs queue */
int chair[3];
int next_seat;

/* student being served */
int curr_student;

/* the numeric id of each student */
int studentIDs[NUM_OF_STUDENTS];

/* student function prototype */
void *student_loop(void *param);
void hang_out(int lnumber, int sleep_time);

/* ta function prototype */
void *ta_loop(void *param);
void help_student(int sleep_time);

