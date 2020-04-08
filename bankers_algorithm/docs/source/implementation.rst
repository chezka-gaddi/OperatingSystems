Implementation
==============

Banker's Algorithm
------------------



Safety Algorithm
________________

1. Let **Work** and **Finish** be vectors of length *m* and *n*, respectively. Initialize
   **Work** = **Available** and **Finish** [*i*] = 0 for *i* = 0, 1,.., *n* - 1.

2. Find an index *i* such that both

   a. **Finish** [*i*] == 0

   b. **Need**\ :sub:`i` <= **Work**

3. **Work** = **Work** + **Allocation**\ :sub:`i`
   
   **Finish** [*i*] = *true*
   
   Go to step 2

4. If **Finish** [*i*] == *true* for all *i*, then the system is in a safe state.


Resource-Request Algorithm
__________________________

1. If **Request**\ :sub: `i` <= **Need**\ :sub:`i`, go to step 2. Otherwise, raise an error condition, since the thread has exceeded its maximum claim.

2. If **Request**\ :sub: `i` <= **Available**, go to step 3. Otherwise, *T*\ :sub:`i` must wait, since the resources are not available.
   
3. Have the system pretend to have allocated the requested resources to thread *T*\ :sub:`i` by modifying the state as follows:

   **Available** = **Available** - **Request**\ :sub:`i`

   **Allocation**\ :sub:`i` = **Allocation**\ :sub:`i` + **Request**\ :sub:`i` 
   
   **Need**\ :sub:`i` = **Need**\ :sub:`i` + **Request**\ :sub:`i`
   
   If the resulting resource-allocation state is safe, the transaction is completed, and thread *T*\ :sub:`i` is allocated its resources. However, if the new state is unsafe, then *T*\ :sub:`i` must wait for **Request**\ :sub:`i`, and the old resource-allocation state is restored.
