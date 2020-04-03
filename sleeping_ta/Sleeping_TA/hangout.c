/**
 * Simulate a student hanging out
 */

#include <stdio.h>
#include "ta.h"

/**
 * Function to simulate a seperate activity that the
 * student decides to partake in while waiting for
 * Austin to go through the mass of students always
 * wanting help.
 *
 * @param[in] lnumber - student ID number
 * @param[in] sleep_time - time to kill
 */
void hang_out(int lnumber, int sleep_time) 
{
    printf("\tStudent %d hanging out for %d seconds\n", lnumber, sleep_time);
    sleep(sleep_time);
}
