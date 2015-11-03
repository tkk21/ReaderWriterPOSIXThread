#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 200
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
    readcount++;
    fflush(stdout);
    printf("Thread %d read \treadcount: %d", data->tid, readcount);
    if (readcount==1){
        wait(&wrt);
    }
    printf("Thread %d releases", data->tid);
    fflush(stdout);
    signal(&mutex);
    //reading
    wait(&mutex);
    readcount--;
    if (readcount==0){
        signal(&wrt);
    }
    signal(&mutex);
}

void *write(void *args){
    thread_data *data = (thread_data *)args;
    wait(&wrt);
    //writing
    signal(&wrt);
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
        printf("debug)");
    }
    else{
        int i;
        for (i = 0; i<NUM_THREADS; i++){
            void *thread_function;//pointer to the thread to call
            
            thread_data_arr[i].tid = i; //set the id of the thread
            
            if (getRand()==0){//read
                thread_function = read;
            }
            else{/write
                thread_function = write;
            }

            //create the thread
            if ((errorCheckThread = pthread_create(&threads[i], NULL, thread_function, &thread_data_arr[i]))){
                fprintf(stderr, "error: pthread_create, %d\n", errorCheckThread);
                return EXIT_FAILURE;
            }
        }

        //join threads

        for (i=0; i<NUM_THREADS; i++){
            if ((errorCheckThread = pthread_join(&threads[i], NULL))){
                fprintf(stderr, "error: pthread_join, %d\n", errorCheckThread);
            }
        }
    }
    return 0;
}

