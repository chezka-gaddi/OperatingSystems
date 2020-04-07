/* customers.h */

void *customer_loop(void *param);

int request_resources(int customer_num, int request[]);
int release_resources(int customer_num, int release[]);
int calculate_need(int customer_num);
