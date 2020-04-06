/* bankers.c */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "bank.h"
#include "customers.h"

pthread_t customers[NUMBER_OF_CUSTOMERS];

/**
 * Initialize all relevant data structures and synchronization objects.
 **/
void init(const char * argv[])
{
    int i = 0;
    int j = 0;

    // available = available - request
    // request = 0
    while (i < NUMBER_OF_RESOURCES)
    {
        available[i] = atoi(argv[i+1]);
        i++;
    }

    // allocation = allocation + request
    // allocation = 0
    // need = max - allocation
    for (i=0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        for (j=0; j < NUMBER_OF_RESOURCES; j++)
        {
            allocation[i][j] = 0;
            maximum[i][j] = rand() % (available[i] + 1);
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }

    /* initialize mutex lock */
    if (pthread_mutex_init(&mutex, NULL) != 0)
        printf("%s\n", strerror(errno));
    
    /* initialize customer IDs */
    for (i=0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        customerIDs[i] = i;
    }
}

void create_customers()
{
    int i;
    for (i=0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        pthread_create(&(customers[i]), NULL, &customer_loop, (void*) &customerIDs[i]);
    }
}

void print_state()
{
    //printf("Processes (currently allocated resources):\n");
    //printf("   %s\n", string);
    int i, j;
    
	printf("    Allocation     Need    Available\n");
	printf("     A  B  C     A  B  C    A  B  C\n");
	for (i=0; i < NUMBER_OF_CUSTOMERS; i++)
	{
        printf("P%d  ", i);
        for (j=0; j < NUMBER_OF_RESOURCES; j++)
		{
            printf("%2d ", allocation[i][j]);
        }
		
		printf("   ");
        for (j=0; j < NUMBER_OF_RESOURCES; j++)
		{
            printf("%2d ", need[i][j]);
        }
		
		printf("  ");
		if (i==0)
		{
			for (j=0; j < NUMBER_OF_RESOURCES; j++)
            	printf("%2d ", available[j]);
		}
 
        printf("\n");
    }
    
}

int main(int argc, const char * argv[])
{
    int i;
    if (argc != NUMBER_OF_RESOURCES + 1)
    {
        printf("Incorrect number of resources.");
        exit(0);
    }

    init(argv);
    create_customers();

    for (i=0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        pthread_join(customers[i], NULL);
    }
	print_state();
    pthread_mutex_destroy(&mutex);
}

