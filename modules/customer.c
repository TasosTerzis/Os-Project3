#include "../include/mutual.h"

void customer (SharedMemory shm, int K, int L, float l, int id) {

    pid_t pid = getpid();
	srand(pid);     // seed with unique pid

    // create a log file named processI inside log folder.
    char filename[20];
    sprintf(filename, "log/process%d.txt", id);

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("fopen"); exit(1); }
    
    // write initial info to log
    fprintf(file, "This is the log file of process number %d, with Pid: %d\n\n", id, pid);

    // loop to create L requests and place them in the queue
    for(int i = 0; i < L; i++ ) { 

        // print request counter to log
        fprintf(file, "--------------------------------------------------");
        fprintf(file, "----------------------------------------------\n\n");
        fprintf(file, "Request %d:\n", i+1);

        // form a request                    
        Request request;
        request.fileNum = rand() % K;               // random file number (0-9)
        request.start = rand() % TOTAL_LINES;       // random start line
        request.stop = rand() % TOTAL_LINES;        // random stop line
        if(request.start >= request.stop) {         // if start > stop, swap them
            int temp = request.start;
            request.start = request.stop;
            request.stop = temp;
        }

        // create temporary shared memory to hold the buffer
        int shmTempId = shmget(IPC_PRIVATE, sizeof(TempSharedMemory), IPC_CREAT | 0666);
        if (shmTempId == -1) {
            perror("shmget"); exit(1);}

        // Attach shared memory segment
        TempSharedMemory shmTemp = (TempSharedMemory)shmat(shmTempId, NULL, 0);
        if (shmTemp ==(TempSharedMemory)(-1)) {
            perror("shmat"); exit(1);}
        
        // add shmTempId to request so the serving thread can attach to the new temp shared memory
        request.shmTempId = shmTempId;
        if(sem_init(&shmTemp->dataReady, 1, 0) == -1) {
            perror("sem_init"); exit(1);}
        if(sem_init(&shmTemp->dataEaten, 1, 0) == -1) {
            perror("sem_init"); exit(1);}

        // Wait for empty slot in shared memory buffer
        sem_wait(&shm->empty);
        
        //exclusive access to queue
        sem_wait(&shm->mutex);
        // Put request in shared memory
        shm->queue[shm->in] = request;
        shm->in = (shm->in + 1) % QUEUE_SIZE;
        gettimeofday(&request.requestTime, NULL); // set the request time.
        sem_post(&shm->full);
        sem_post(&shm->mutex);

        // start retrieving data from temp shared memory
        int num_blocks = request.stop-request.start;
        for (int block = 0; block <= num_blocks; block++) {
            
            // Wait for server thread to write new block
            sem_wait(&shmTemp->dataReady);
            // Read data_block from shared memory segment
            char data_block[BLOCK_SIZE];
            memcpy(data_block, shmTemp->block, BLOCK_SIZE);
            //  print data block to log file
            fprintf(file, "%s", data_block);
            // Notify the server process that the block has been read
            sem_post(&shmTemp->dataEaten);
        
        }

        // Add request info to log file
        fprintf(file, "\nSTATS:\n");
        fprintf(file, "FileNum: %d, Start: %d, Stop: %d, \nRequest arrived at: %ld\n",
        request.fileNum, request.start, request.stop, request.requestTime.tv_usec);
        struct timeval finishTime;
        gettimeofday(&finishTime, NULL);
        fprintf(file, "Request served at: %ld\n\n", finishTime.tv_usec);

        // destroy semaphores, detach from shared memory segment and delete it
        sem_destroy(&shmTemp->dataReady);
        sem_destroy(&shmTemp->dataEaten);
        shmdt(shmTemp);
        shmctl(shmTempId, IPC_RMID, NULL);

        // simulation of:  "Among the requests exists an exponentially distributed time" 
        double u = (double)rand() / RAND_MAX;
        double time_between_requests = -log(1.0-u)/(double) l; 
        usleep(time_between_requests);

    }

}







