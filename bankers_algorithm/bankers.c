/* bankers.c */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "bank.h"

int main(int argc, const char * argv[])
{
    int i = 0;

    if (argc != NUMBER_OF_RESOURCES + 1)
    {
        printf("Incorrect number of resources.");
        exit(0);
    }

    while (i < NUMBER_OF_RESOURCES)
    {
        available[i] = atoi(argv[i+1]);
        i++;
    }
    
}
