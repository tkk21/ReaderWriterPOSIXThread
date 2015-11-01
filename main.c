#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 200


typedef struct _thread_data_ {
    int tid;// thread's id
} thread_data;

sem_t mutex, wrt;
int readcount=0; // status of whether or not something was read or not

// Error checked syscall of semaphore wait
void wait(sem_t *sem){
    if(sem_wait(sem) < 0) {
        perror ("sem_wait");
        exit (EXIT_FAILURE);
    }
}

// Error checked syscall of semaphore signal
void signal(sem_t *sem){
    if(sem_post(sem) < 0){
        perror ("sem_post");
        exit (EXIT_FAILURE);
    }
}

void *read(void *args){
    thread_data *data = (thread_data *)args;
    wait(&mutex);
    
    
    signal(&mutex);
}

void *write(void *args){
    thread_data *data = (thread_data *)args;
}

// Initializing the semaphores
void initialize_semaphores(){
    if (sem_init(&mutex, 0, (unsigned int)1)<0){
        perror("sem_init(mutex)");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&wrt, 0, (unsigned int)1)<0){
        perror("sem_init(wrt)");
        exit(EXIT_FAILURE);
    }
}

int main (){
    pthread_t threads[NUM_THREADS];
    thread_data thread_data_arr[NUM_THREADS];
    

    return 0;
}

