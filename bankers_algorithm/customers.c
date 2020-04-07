/* customers.c */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "bank.h"
#include "customers.h"


/**
 * The customer loop first creates a request for a random number of resources
 * dependent on their need. They send the request and keep sending that request
 * until it is either met, or they reached the max number of request.
 *
 * The loop also releases a random number resources, and releases all of of the 
 * customer's allocated resources when their need goes to 0.
 *
 * :param param: The customer id
 * :return: 0 on success
 */
void *customer_loop(void *param)
{
    int id = *(int*) param;
    int i;
    int times_through_loop = 0;
    int request_sum = 0;
    int need_status = 0;

    while (need_status == 0)
    {
        int request[NUMBER_OF_RESOURCES] = {0};
       
        /* makes sure that the request is not 0 */
        while (request_sum == 0)
        {
            request_sum = 0;
            for (i=0; i < NUMBER_OF_RESOURCES; i++)
            {
                request[i] = (random() % (need[id][i]+ 1));
                request_sum += request[i];
            }
        }
        request_sum = 0;
        
        /* stop requesting after 5 consecutive failed requests */

        int request_status = -1;
        while (times_through_loop < 5 && request_status == -1)
        {
            request_status = request_resources(id, request);
            times_through_loop++;
        }
        times_through_loop = 0;

        /* release resources when request has failed */
        if (request_status == -1)
        {
            for (i=0; i < NUMBER_OF_RESOURCES; i++)
            {
                request[i] = (random() % (allocation[id][i]+ 1));
            }
            
            release_resources(id, request);
        }

        /* mark process as finished when need is 0 */
        if (calculate_need(id) == 0)
        {
            release_resources(id, need[id]);
            need_status = 1;
        }
    }

    return 0;
}

/**
 * request_resources obtains the mutex lock to stage sending the request for
 * resources to the bank. If the request is more than what is currently
 * available or if it exceeds the needs of the customer, the request fails.
 *
 * It sends the request to the safety_test to ensure that the request is safe
 * and sends the request through when approved.
 *
 * It then prints the current state of the system and unlocks the mutex.
 *
 * :param customer_num: number of requesting customer
 * :param request: number of resources to be requested
 * :return: 0 if request went through, -1 if request is denied
 */
int request_resources(int customer_num, int request[])
{
    int i;
    pthread_mutex_lock(&mutex);

    printf("\nRequest P%d <", customer_num);
    for (i=0; i < NUMBER_OF_RESOURCES; i++)
    {
        printf(" %d", request[i]); 
    }
    printf(" >\n");

    for (i=0; i < NUMBER_OF_RESOURCES; i++)
    {
        if (request[i] > available[i])
        {
            pthread_mutex_unlock(&mutex);
            return -1;
        }

        if (request[i] > need[customer_num][i])
        {
            pthread_mutex_unlock(&mutex);
            return -1;
        }
    }

    if (safety_test(customer_num, request) == 0)
    {
        printf("Safe, request granted.\n");        
        
        for (i=0; i < NUMBER_OF_RESOURCES; i++)
        {
            allocation[customer_num][i] = allocation[customer_num][i] + request[i];
            available[i] = available[i] - request[i];
            need[customer_num][i] = need [customer_num][i] - request[i];
        }
        print_state();
    }

    pthread_mutex_unlock(&mutex);
    return 0;
}

/**
 * release_resources locks the mutex and then adds the release array to the
 * available array and subtracts the release array to the customer's allocation
 * array.
 *
 * :param customer_num: number of the customer releasing resources
 * :param release: number of resources to be released
 * :return: 0 if successful, -1 if unsuccessful
 */
int release_resources(int customer_num, int release[])
{
    int i;
    pthread_mutex_lock(&mutex);
    
    printf("\nP%d releasing resources <", customer_num);        
    for (i=0; i < NUMBER_OF_RESOURCES; i++)
    {
        printf(" %d", release[i]);
        available[i] += allocation[customer_num][i];
        allocation[customer_num][i] -= release[i];
    }
    printf(" >\n");
    print_state();
    
    pthread_mutex_unlock(&mutex);
    return 0;
}

/**
 *  Calculates the total number of resources that a given customer still needs.
 *
 * :param customer_num: customer number to be evaluated
 * :return: the total number of resources the customer still needs
 */
int calculate_need(int customer_num)
{
    int needSum = 0;
    int i;

    for (i=0; i < NUMBER_OF_RESOURCES; i++)
    {
        needSum += need[customer_num][i];
    }

    return needSum;
}
