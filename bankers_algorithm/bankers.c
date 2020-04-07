/* bankers.c */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "bank.h"
#include "customers.h"

pthread_t customers[NUMBER_OF_CUSTOMERS];

/**
 * Initialize all relevant data structures and synchronization objects.
 *
 * Initializes the available array to the command line inputs, the
 * allocation array to 0, the maximum array to a random number dependent on the
 * available array and the need array to the maximum - allocation.
 *
 * The mutex lock is also initialized as well as the customer ID numbers.
 *
 * :param argv: command line inputs of the number of available resources.
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
            maximum[i][j] = (random() % (available[j] + 1));
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

/**
 * Create all of the customer threads.
 */
void create_customers()
{
    int i;
    for (i=0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        pthread_create(&(customers[i]), NULL, &customer_loop, (void*) &customerIDs[i]);
    }
}

/**
 * safety_test first makes a copy of the current state to run tests on. It then
 * applies the request by subtracting the request array from the allocation
 * array and adds the request array to the customer's allocation array. It then
 * follows the Banker's Algorithm with the safety test.
 *
 * :param customer: number of the customer making the request
 * :param request: resources being requested
 * :return: 0 if safe state is found, -1 if safe state not found
 */
int safety_test(int customer, int request[])
{
    int i,j;
    
    /* make a copy of the current state */
    for (i=0; i < NUMBER_OF_RESOURCES; i++)
    {
        available_test[i] = available[i];
        for (j=0; j < NUMBER_OF_CUSTOMERS; j++)
        {
            allocation_test[j][i] = allocation[j][i];
            maximum_test[j][i] = maximum_test[j][i];
            need_test[j][i] = need [j][i];
            finish[j] = 0;
        }
    }

    /* stage the request going through */
    for (i=0; i < NUMBER_OF_RESOURCES; i++)
    {
        available_test[i] -= request[i];
        work[i] = available_test[i];
        allocation_test[customer][i] += request[i];
        need_test[customer][i] -= request[i];
    }


    //print_test_state();

    int count = 0;
    int iterations = 0;
    int status = 1;
    while (iterations < NUMBER_OF_CUSTOMERS)
    {
        for (i=0; i < NUMBER_OF_CUSTOMERS; i++)
        {
            if (finish[i] == 0)
            {
                //printf("Checking %d.\n", i);
                //print_test_state();
                for (j=0; j < NUMBER_OF_RESOURCES; j++)
                {
                    if (need_test[i][j] > work[j])
                    {
                        status = 0;
                    }
                }

                if (status == 1)
                {
                    //printf("%d is finish.\n", i);
                    for (j=0; j < NUMBER_OF_RESOURCES; j++)
                        work[j] += allocation_test[i][j];
                    finish[i] = 1;
                    count++;
                }
                status = 1;
            }
        }

        if (count == NUMBER_OF_CUSTOMERS)
        {
            return 0;
        }

        iterations++;
    }

    printf("Safe state not found.\n\n");
    return -1;
}

/**
 * Prints the contents of the test arrays.
 */
void print_test_state()
{
    int i, j;
    
    printf("\n-------------Test State-------------\n");
	printf("    Allocation     Need       Work\n");
	printf("     A  B  C     A  B  C    A  B  C\n");
	for (i=0; i < NUMBER_OF_CUSTOMERS; i++)
	{
        printf("P%d  ", i);
        for (j=0; j < NUMBER_OF_RESOURCES; j++)
		{
            printf("%2d ", allocation_test[i][j]);
        }
		
		printf("   ");
        for (j=0; j < NUMBER_OF_RESOURCES; j++)
		{
            printf("%2d ", need_test[i][j]);
        }
		
		printf("  ");
		if (i==0)
		{
			for (j=0; j < NUMBER_OF_RESOURCES; j++)
            	printf("%2d ", work[j]);
		}
 
        printf("\n");
    }
    
}

/**
 * Prints the current state of the system.
 */
void print_state()
{
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

/**
 * Initializes the matrices, prints the initial state of the system and syncs
 * the customer threads.
 */
int main(int argc, const char * argv[])
{
    int i;
    if (argc != NUMBER_OF_RESOURCES + 1)
    {
        printf("Incorrect number of resources.");
        exit(0);
    }

    init(argv);
	print_state();
    create_customers();

    for (i=0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        pthread_join(customers[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
}

