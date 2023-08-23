#include "../include/mutual.h"

void customer (SharedMemory shm, int K, int L, float l) {

    pid_t pid = getpid();
	srand(pid);

    for(int i = 0; i < L; i++ ) { 

        // form a request                    
        Request request;
        request.pid = pid;
        request.fileNum = rand() % K;
        request.start = rand() % TOTAL_LINES;
        request.stop = rand() % TOTAL_LINES;
        if(request.start > request.stop) {
            int temp = request.start;
            request.start = request.stop;
            request.stop = temp;
        }

        // simulate:  "Among the requests an exponentially distributed time" 
        double u = (double)rand() / RAND_MAX;
        double time_between_requests = -log(1.0 - u) / (double) l;

        // Wait for empty slot in shared memory buffer
        sem_wait(&(shm->empty));
        sem_wait(&(shm->mutex));

        // Put request in shared memory
        shm->array[shm->in] = request;
        shm->in = (shm->in + 1) % MAX_QUEUE_SIZE;
        shm->size++;
        gettimeofday(&request.requestTime, NULL); // set request time since
        printf("RequestTime: %ld, PID: %d, FileNum: %d, Start: %d, Stop: %d\n", 
        request.requestTime.tv_usec, request.pid, request.fileNum, request.start, request.stop);
        sem_post(&shm->mutex);
        sem_post(&shm->full);

        // DONT FORGET TO UNCOMMENT THIS. 
        // usleep(time_between_requests * 1000000);

    }

}







