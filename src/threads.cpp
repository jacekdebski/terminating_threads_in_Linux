#include "../include/threads.h"

int allocate_memory(thread_data **td, int number_of_threads) {
    *td = (thread_data *) malloc(sizeof(thread_data));
    if (*td != NULL) {
        (*td)->thread_id_array = (pthread_t *) malloc(sizeof(pthread_t) * number_of_threads);
        (*td)->start_mutex = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
        (*td)->sem_threads_start = (sem_t *) malloc(sizeof(sem_t));
        if ((*td)->thread_id_array == NULL || (*td)->start_mutex == NULL || (*td)->sem_threads_start == NULL) {
            deallocate_memory(*td);
            return ERROR_ALLOCATION_MEMORY;
        } else {
            (*td)->thread_to_finish = NULL;
        }
    }
    return SUCCESS;
}

void deallocate_memory(thread_data *td) {
    if (td != NULL) {
        if (td->thread_id_array != NULL) {
            free(td->thread_id_array);
        }
        if (td->start_mutex != NULL) {
            free(td->start_mutex);
        }
        if (td->sem_threads_start != NULL) {
            free(td->sem_threads_start);
        }
        free(td);
        td = NULL;
    }
}

void *foo(void *args) {
    const pthread_t current_thread = pthread_self();
    sem_wait(((thread_data *) args)->sem_threads_start);
    while (1) {
        if (((thread_data *) args)->thread_to_finish != NULL &&
            current_thread == *(((thread_data *) args)->thread_to_finish)) {
            pthread_mutex_lock(((thread_data *) args)->start_mutex);
            std::cout << "Thread with ID: " << *(((thread_data *) args)->thread_to_finish) << " is finishing"
                      << std::endl;
            pthread_mutex_unlock(((thread_data *) args)->start_mutex);
            break;
        } else {
            sleep(1);
        }
    }
    return NULL;
}

int createThreads(struct thread_data **td, int number_of_threads) {
    int res;
    res = allocate_memory(td, number_of_threads);
    if (res == SUCCESS) {
        sem_init((*td)->sem_threads_start, 0, 0);
        pthread_mutex_init((*td)->start_mutex, NULL);
        for (int i = 0; i < number_of_threads; i++) {
            res = pthread_create((*td)->thread_id_array + i, NULL, foo, *td);
            if (res != SUCCESS) {
                std::cout << "Thread creation failed" << std::endl;
                deallocate_memory(*td);
                sem_destroy((*td)->sem_threads_start);
                pthread_mutex_destroy((*td)->start_mutex);
                return ERROR_CREATION_THREAD;
            } else {
                std::cout << "Thread has created with ID: " << *((*td)->thread_id_array + i) << std::endl;
            }
        }
        for (int i = 0; i < number_of_threads; ++i) {
            sem_post((*td)->sem_threads_start);
        }
    } else {
        std::cout << "Failed to allocate memory." << std::endl;
    }
    return res;
}

void finishThreads(struct thread_data *td, int number_of_threads,
                   enum order_of_finishing_threads direction_finishing_threads) {
    int res;
    if (direction_finishing_threads == dec) {
        for (int i = number_of_threads - 1; i >= 0; --i) {
            td->thread_to_finish = td->thread_id_array + i;
            res = pthread_join(*(td->thread_id_array + i), NULL);
            if (res != 0) {
                std::cout << "Thread with ID: " << *(td->thread_id_array + i) << " join failed" << std::endl;
            } else {
                std::cout << "Thread with ID: " << *(td->thread_id_array + i) << " has finished execution" << std::endl;
            }
        }
    } else if (direction_finishing_threads == inc) {
        for (int i = 0; i < number_of_threads; ++i) {
            td->thread_to_finish = td->thread_id_array + i;
            res = pthread_join(*(td->thread_id_array + i), NULL);
            if (res != 0) {
                std::cout << "Thread with ID: " << *(td->thread_id_array + i) << " join failed" << std::endl;
            } else {
                std::cout << "Thread with ID: " << *(td->thread_id_array + i) << " has finished execution" << std::endl;
            }
        }
    }
    sem_destroy(td->sem_threads_start);
    pthread_mutex_destroy(td->start_mutex);
    deallocate_memory(td);
}

void display_help() {
    std::cout << "First argument: number of threads (3-100)" << std::endl;
    std::cout << "Second argument: order of finishing threads (inc, dec)" << std::endl;
}