Introduction
============

Overview
--------

This is a multi-threaded program that implements the Banker's Algorithm. Several customers request and release resources from the bank. The banker will grant a request only if it leaves the system in a safe state. A request that leaves the system in an unsafe state will be denied. This programming assignment combines three separate topics:
(1) multi-threading, (2) preventing race conditions, and (3) deadlock avoidance.



Running the Application
-----------------------

The program is built by running *make* in the main project folder.

The program is invoked by passing the number of resources of each type on the command line.

.. code-block:: shell

	./bankers 10 5 7

The *available* array is initialized with these values. Currently, the program will take in initial values for three resources.
