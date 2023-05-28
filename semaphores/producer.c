// To run the code, first run consumer in the background (./consumer &). Then run the producer. 
//To stop the program, type 'end' and press enter.

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <semaphore.h>

#define BUFFER_SIZE 10
#define TEXT_SZ 2048

struct shared_use_st {
    char some_text[TEXT_SZ][BUFFER_SIZE];
};

// define semaphore names
#define SEMAPHORE_NAME_EMPTIES "/ourSemaphore1.dat"
#define SEMAPHORE_NAME_FULLS "/ourSemaphore2.dat"

int main() {
    void *shared_memory = (void *)0;
    struct shared_use_st *shared_stuff;
    int shmid;
    char msg[BUFSIZ];

    // initialize the semaphores
    sem_t *semIDE;
    sem_t *semIDF;
    semIDE = sem_open(SEMAPHORE_NAME_EMPTIES, 0);
    semIDF = sem_open(SEMAPHORE_NAME_FULLS, 0);

    // check for errors in semaphore initialization
    if (semIDE == SEM_FAILED || semIDF == SEM_FAILED) {
        perror("Error opening semaphores");
        exit(EXIT_FAILURE);
    }

    srand((unsigned int)getpid());

    // create a shared memory segment
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    // attach the shared memory segment so it is available to the program
    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Memory attached at %p\n", shared_memory);
    shared_stuff = (struct shared_use_st *)shared_memory;

    int running = 1;
    int write_index = 0;

    while (running) {
        
        if (sem_wait(semIDE) != 0)
            perror("sem_wait error");

        // copy entered message into the shared memory
        printf("Enter some text: ");
        fgets(msg, BUFSIZ, stdin);
        strncpy(shared_stuff->some_text[write_index], msg, BUFSIZ);

        // break the loop if the user enters 'end'
        if (strncmp(msg, "end", 3) == 0) {
            running = 0;
        }

        write_index = (write_index + 1) % TEXT_SZ;

        if (sem_post(semIDF) != 0)
            perror("sem_post error");
    }

    // detach the shared memory segment
    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
