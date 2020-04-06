/* customers.c */

#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "bank.h"
#include "customers.h"


void *customer_loop(void *param)
{
    int id = *(int*) param;
    
    return 0;
}
