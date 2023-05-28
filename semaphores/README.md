# Producer-Consumer Problem

This repository contains three programs that demonstrate the Producer-Consumer problem, which involves two threads, a producer and a consumer, accessing a shared buffer concurrently. The producer produces items and adds them to the buffer, while the consumer consumes items from the buffer.

## Prerequisites
To run these programs, you need a Linux system with a C compiler and the necessary development libraries. Make sure you have the following libraries installed:

* libc-dev
* libpthread-dev
* libsemaphore-dev (required for the semaphore-based implementation)

## Program 1: Shared Memory Producer-Consumer

The files producer.c and consumer.c implement the producer and consumer using shared memory and semaphores for synchronization.

To run the program:

* Compile the producer program using the following command: ***gcc producer_shared_memory.c -o producer***

* Compile the consumer program using the following command: ***gcc consumer_shared_memory.c -o consumer***

* Run the consumer program in the background: ***./consumer &***

* Run the producer program: ***./producer***

* To stop the program, type 'end' and press Enter.

***

## Program 2: Mutex and Semaphores Producer-Consumer

The file mutex_semaphores.c implements the producer and consumer using mutex locks and semaphores for synchronization.

To run the program:

* Compile the program using the following command: ***gcc mutex_semaphores.c -o producer_consumer -pthread***

* Run the program: ***./producer_consumer***
