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

        // create a temporary shared memory segment of size TempSharedMemory
        int shmTempId = shmget(IPC_PRIVATE, sizeof(TempSharedMemory), IPC_CREAT | 0666);
        if (shmTempId == -1) {
            perror("shmget"); exit(1);}
        // Attach shared memory segment
        TempSharedMemory shmTemp = (TempSharedMemory)shmat(shmTempId, NULL, 0);
        if (shmTemp ==(TempSharedMemory)(-1)) {
            perror("shmat"); exit(1);}
        
        // initialize shared memory
        if (sem_init(&shmTemp->mutex, 1, 1) == -1) {
            perror("sem_init"); exit(1);}
        


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
        // printf("RequestTime: %ld, PID: %d, FileNum: %d, Start: %d, Stop: %d\n", 
        // request.requestTime.tv_usec, request.pid, request.fileNum, request.start, request.stop);
        sem_post(&shm->mutex);
        sem_post(&shm->full);


        // start retrieving data from temp shared memory
        int num_blocks = request.stop-request.start;
        num_blocks = 1;
        for (int block = 0; block < num_blocks; block++) {
            // Wait for server process to write new block
            sem_wait(&(shmTemp->mutex));

            // Read data_block from shared memory segment
            char data_block[BLOCK_SIZE];
            memcpy(data_block, shmTemp->array, BLOCK_SIZE);
            printf("Data Block: %s\n", data_block);

            // Notify the server process that the block has been read
            sem_post(&(shmTemp->mutex));
        }


        // DONT FORGET TO UNCOMMENT THIS. 
        // usleep(time_between_requests * 1000000);

    }

}







