#include "../include/mutual.h"

void customer (SharedMemory shm, int K, int L, int l) {

    // pid_t pid = getpid();
	// srand(pid);

    printf("Hi i'm process %d\n", getpid());
    // for(int i=0; i<L; i++) {
                
    //     Request request = (Request)malloc(sizeof(struct request));
    //     request->pid = pid;
    //     request->fileNum = rand() % K;
    //     request->start = rand() % TOTAL_LINES;
    //     request->stop = rand() % TOTAL_LINES;
    //     request->next = NULL;   // for now

        
    //     // use semaphores to access the queue
    //     sem_wait(&shm->queueSem);
    //     enqueue(shm->queue, request);
    //     printf("queue size: %d\n", queueSize(shm->queue));
    //     sem_post(&shm->queueSem);

    //     // Among the requests an exponentially distributed time (parameter λ defined as simulation start parameter
    //     // since λ is given, we'll simulate the exponential time by sleeping for a random time between 0 and l

    // }
    if(shm->test==10) {
        //increase test
        shm->test = 20;
    } 


    // printQueue(shm->queue);
}