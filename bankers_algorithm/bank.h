/* bank.h */

/* these may be any values >= 0 */
#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

pthread_mutex_t mutex;
int customerIDs[NUMBER_OF_CUSTOMERS];

/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];

/* the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* variables for the safety test */
int finish[NUMBER_OF_CUSTOMERS];
int work[NUMBER_OF_RESOURCES];
int available_test[NUMBER_OF_RESOURCES];
int allocation_test[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int maximum_test[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need_test[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

void *bank_loop(void *param);

int safety_test(int customer_num, int request[]);

void print_state();
void print_test_state();
