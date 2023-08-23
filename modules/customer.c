#include "../include/mutual.h"

void customer (SharedMemory shm, int K, int L, int l) {

    pid_t pid = getpid();
	srand(pid);

    printf("i'll create %d requests\n", L);
    for(int i = 0; i < L; i++ ) { 

        // form a request                    
        Request request;
        request.pid = pid;
        request.fileNum = rand() % K;
        request.start = rand() % TOTAL_LINES;
        request.stop = rand() % TOTAL_LINES;

        // use semaphores to access the queue and add the request
        sem_wait(&shm->queueSem);
        enqueue(&shm->queue, request);
        sem_post(&shm->queueSem);
        sem_post(&shm->nonEmpty);   // notify server that queue is not empty

        // start

    //     // Among the requests an exponentially distributed time (parameter λ defined as simulation start parameter
    //     // since λ is given, we'll simulate the exponential time by sleeping for a random time between 0 and l

    }

}