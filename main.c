#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct _thread_data_ {
    int tid;// thread's id
    int readcount; // status of whether or not something was read or not
}
