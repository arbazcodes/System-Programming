#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 20
#define NUM_ITEMS 30

int buffer[BUFSIZ];
int buffer_index = 0;

sem_t empty_sem, full_sem;
pthread_mutex_t mutex;

void *producer(void *arg) {
    int item;
    for (int i = 0; i < NUM_ITEMS; i++) {
        item = rand() % 100;  // Generate a random item

        sem_wait(&empty_sem);  // Wait for an empty slot in the buffer
        pthread_mutex_lock(&mutex);  // Acquire the mutex lock

        buffer[buffer_index] = item;
        buffer_index = (buffer_index + 1) % BUFFER_SIZE;
        printf("Produced item: %d\n", item);

        pthread_mutex_unlock(&mutex);  // Release the mutex lock
        sem_post(&full_sem);  // Signal that the buffer is full
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int item;
    for (int i = 0; i < NUM_ITEMS; i++) {
        sem_wait(&full_sem);  // Wait for a full slot in the buffer
        pthread_mutex_lock(&mutex);  // Acquire the mutex lock

        item = buffer[(buffer_index - 1 + BUFFER_SIZE) % BUFFER_SIZE];
        buffer_index = (buffer_index - 1 + BUFFER_SIZE) % BUFFER_SIZE;
        printf("Consumed item: %d\n", item);

        pthread_mutex_unlock(&mutex);  // Release the mutex lock
        sem_post(&empty_sem);  // Signal that the buffer is empty
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Initialize semaphores and mutex
    sem_init(&empty_sem, 0, BUFFER_SIZE);
    sem_init(&full_sem, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy semaphores and mutex
    sem_destroy(&empty_sem);
    sem_destroy(&full_sem);
    pthread_mutex_destroy(&mutex);

    return 0;
}
