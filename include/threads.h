#ifndef TERMINATING_THREADS_IN_LINUX_H
#define TERMINATING_THREADS_IN_LINUX_H

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <semaphore.h>

#define ERROR 1
#define ERROR_ALLOCATION_MEMORY -1
#define ERROR_CREATION_THREAD -2
#define SUCCESS 0

enum order_of_finishing_threads {
    inc,
    dec
};

struct thread_data {
    pthread_t *thread_id_array;
    pthread_t *thread_to_finish;
    pthread_mutex_t *start_mutex;
    sem_t *sem_threads_start;
};


int allocate_memory(thread_data **td, int number_of_threads);

void deallocate_memory(thread_data *td);

void *foo(void *args);

int createThreads(struct thread_data **td, int number_of_threads);

void finishThreads(struct thread_data *td, int number_of_threads,
                   enum order_of_finishing_threads direction_finishing_threads);

void display_help();

#endif //TERMINATING_THREADS_IN_LINUX_H
