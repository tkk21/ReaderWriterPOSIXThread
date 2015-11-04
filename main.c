#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 10
#define RAND_RANGE 2

typedef struct _thread_data_ {
    int tid;// thread's id
} thread_data;
sem_t mutex, wrt;
int readcount=0; // status of whether or not something was read or not
time_t t;

int getRand(){
    return  rand() % RAND_RANGE;
}

// Error checked syscall of semaphore wait
void semwait(sem_t *sem){
    if(sem_wait(sem) < 0) {
        perror ("sem_wait");
        exit (EXIT_FAILURE);
    }
}

// Error checked syscall of semaphore signal
void semsignal(sem_t *sem){
    if(sem_post(sem) < 0){
        perror ("sem_post");
        exit (EXIT_FAILURE);
    }
}


void *reader(void *args){
    thread_data *data = (thread_data *)args;
    
    semwait(&mutex);
    readcount++;
    fflush(stdout);
    printf("[Thread %d] reads \treadcount: %d\n", data->tid, readcount);
    if (readcount==1){
        printf("[Thread %d] reader waits for the writer\n", data->tid);
        fflush(stdout);
        semwait(&wrt);
    }
    fflush(stdout);
    printf("[Thread %d] releases the mutex\n", data->tid);
    fflush(stdout);
    semsignal(&mutex);
    //reading
    semwait(&mutex);
    readcount--;
    fflush(stdout);
    printf("[Thread %d] read \t readcount: %d\n", data->tid, readcount);
    if (readcount==0){
        printf("[Thread %d] reader signals wrt\n", data->tid);
        fflush(stdout);
        semsignal(&wrt);
    }
    printf("[Thread %d] reader releases the mutex\n", data->tid);
    fflush(stdout);
    semsignal(&mutex);
}

void *writer(void *args){
    thread_data *data = (thread_data *)args;
    fflush(stdout);
    printf("[Thread %d] writer waits for the reader\n", data->tid);
    fflush(stdout);
    semwait(&wrt);
    //writing
    fflush(stdout);
    printf("[Thread %d] writer is done writing and signals wrt\n", data->tid);
    fflush(stdout);
    semsignal(&wrt);
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

int main (int argc, char const *argv[]){
    int debug = argc==2;
    int errorCheckThread;//used to error check the pthread creation
    pthread_t threads[NUM_THREADS];
    thread_data thread_data_arr[NUM_THREADS];
 
    //seeding rng
    srand((unsigned int)time(&t));
    
    initialize_semaphores();
    if (debug){
        printf("debug mode\n");
        printf("using preset threads and operations\n");

    }
    else{
        int i;
        for (i = 0; i<NUM_THREADS; i++){
            void *thread_function;//pointer to the thread to call
            thread_data_arr[i].tid = i; //set the id of the thread
            
            if (getRand()==0){//read
                thread_function = reader;
                printf("[Thread %d] is declared as a Reader\n", i);
            }
            else{//write
                thread_function = writer;
                printf("[Thread %d] is declared as a Writer\n", i);
            }

            //create the thread
            if ((errorCheckThread = pthread_create(&threads[i], NULL, thread_function, &thread_data_arr[i]))){
                fprintf(stderr, "error: pthread_create, %d\n", errorCheckThread);
                return EXIT_FAILURE;
            }
        }

        //join threads
        for (i=0; i<NUM_THREADS; i++){
            if ((errorCheckThread = pthread_join(threads[i], NULL))){
                fprintf(stderr, "error: pthread_join, %d\n", errorCheckThread);
            }
        }
    }
    return 0;
}
