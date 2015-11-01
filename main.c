#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct _thread_data_ {
    int tid;// thread's id
    int readcount; // status of whether or not something was read or not
} thread_data;

sem_t mutex, wrt;

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


}

