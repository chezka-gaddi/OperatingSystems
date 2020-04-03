/**
 * Simulate helping a student
 */

#include <stdio.h>
#include "ta.h"

/**
 * Help a student for a random amount of time.
 *
 * @param[in] sleep_time - amount of time to sleep.
 */
void help_student(int sleep_time)
{
    printf("Helping a student for %d seconds waiting students = %d\n", sleep_time, num_waiting);
    sleep(sleep_time);
}
