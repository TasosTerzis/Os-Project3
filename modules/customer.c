#include "../include/mutual.h"

void customer (SharedMemory shm, int K, int L, float l, int id) {

    pid_t pid = getpid();
	srand(pid);

    // create a log file named processI inside folder log
    char filename[20];
    sprintf(filename, "log/process%d.txt", id);
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("fopen"); exit(1); }
    
    // write the pid of the process in the log file
    fprintf(file, "Log file of process number %d, with Pid: %d\n\n", id, pid);

    for(int i = 0; i < L; i++ ) { 


        // print request counter to log
        fprintf(file, "--------------------------------------------------");
        fprintf(file, "----------------------------------------------\n\n");
        fprintf(file, "Request %d:\n", i+1);

        // form a request                    
        Request request;
        request.fileNum = rand() % K;
        request.start = rand() % TOTAL_LINES;
        request.stop = rand() % TOTAL_LINES;
        if(request.start >= request.stop) {
            int temp = request.start;
            request.start = request.stop;
            request.stop = temp;
        }
        request.pid = pid;
        printf("here is my request: %d %d %d %d\n", request.fileNum, request.start, request.stop, request.pid);

        // create a temporary shared memory segment of size TempSharedMemory
        int shmTempId = shmget(IPC_PRIVATE, sizeof(TempSharedMemory), IPC_CREAT | 0666);
        if (shmTempId == -1) {
            perror("shmget"); exit(1);}

        // add shmTempId to request. that way, the serving thread can attach to the new temp shared memory segment
        request.shmTempId = shmTempId;
        
        // Attach shared memory segment
        TempSharedMemory shmTemp = (TempSharedMemory)shmat(shmTempId, NULL, 0);
        if (shmTemp ==(TempSharedMemory)(-1)) {
            perror("shmat"); exit(1);}
        
        // initialize shared memory
        if (sem_init(&shmTemp->mutex, 1, 1) == -1) {
            perror("sem_init"); exit(1);}
        if(sem_init(&shmTemp->dataReady, 1, 0) == -1) {
            perror("sem_init"); exit(1);}
        if(sem_init(&shmTemp->dataEaten, 1, 0) == -1) {
            perror("sem_init"); exit(1);}

        // simulate:  "Among the requests an exponentially distributed time" 
        double u = (double)rand() / RAND_MAX;
        double time_between_requests = -log(1.0-u)/(double) l;

        // Wait for empty slot in shared memory buffer
        sem_wait(&shm->empty);
        sem_wait(&shm->mutex);

        // Put request in shared memory
        shm->array[shm->in] = request;
        shm->in = (shm->in + 1) % MAX_QUEUE_SIZE;
        shm->size++;
        gettimeofday(&request.requestTime, NULL); // set request time since
        sem_post(&shm->full);
        sem_post(&shm->mutex);


        // start retrieving data from temp shared memory
        int num_blocks = request.stop-request.start;

        // for (int block = 0; block <= num_blocks; block++) {
        //     // Wait for server process to write new block
        //     sem_wait(&shmTemp->dataReady);

        //     // printf  ("sem_wait(mutex) beofre\n");
        //     sem_wait(&shmTemp->mutex);
        //     // printf  ("sem_wait(mutex) after\n");


        //     // Read data_block from shared memory segment
        //     char data_block[BLOCK_SIZE];
        //     memcpy(data_block, shmTemp->array, BLOCK_SIZE);
        //     //  print data block to log
        //     fprintf(file, "%s", data_block);

        //     // Notify the server process that the block has been read
        //     sem_post(&shmTemp->mutex);
        //     sem_post(&shmTemp->dataEaten);
        // }

        // add request info to log
        fprintf(file, "\nSTATISTICS:\n");
        fprintf(file, "RequestTime: %ld, PID: %d, FileNum: %d, Start: %d, Stop: %d, ", 
        request.requestTime.tv_usec, pid, request.fileNum, request.start, request.stop);
        struct timeval finishTime;
        gettimeofday(&finishTime, NULL);
        fprintf(file, "FinishTime: %ld\n\n", finishTime.tv_usec);

        // destroy semaphore
        sem_destroy(&shmTemp->mutex);
        sem_destroy(&shmTemp->dataReady);
        sem_destroy(&shmTemp->dataEaten);

        // Detach from shared memory segment
        shmdt(shmTemp);

        // DONT FORGET TO UNCOMMENT THIS. 
        // usleep(time_between_requests * 1000000);

    }

}







