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
        
        int request_status = -1;
        while (times_through_loop < 5 && request_status == -1)
        {
            request_status = request_resources(id, request);
            times_through_loop++;
        }
        
        times_through_loop = 0;

        if (request_status == -1)
        {
            release_resources(id, request);
        }

        if (calculate_need(id) == 0)
        {
            release_resources(id, request);
            need_status = 1;
        }
    }
    return 0;
}

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
    printf("Available <");
    for (i=0; i < NUMBER_OF_RESOURCES; i++)
    {
        printf(" %d", available[i]); 
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

int release_resources(int customer_num, int release[])
{
    int i;
    pthread_mutex_lock(&mutex);
    
    printf("\nP%d releasing all resources.\n", customer_num);        
    for (i=0; i < NUMBER_OF_RESOURCES; i++)
    {
        available[i] += allocation[customer_num][i];
        allocation[customer_num][i] = 0;
    }

    print_state();
    pthread_mutex_unlock(&mutex);
    return 0;
}

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
