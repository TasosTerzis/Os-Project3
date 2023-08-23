#include "../include/mutual.h"

void* serverThread(void* arg) {
    Request request = *(Request*)arg;

    // Attach to the shared memory segment of the client process
    int shmTempId = shmget(request.pid, sizeof(TempSharedMemory), 0666);
    if (shmTempId == -1) {
        perror("shmget");
        pthread_exit(NULL);
    }

    TempSharedMemory shmTemp = (TempSharedMemory)shmat(shmTempId, NULL, 0);
    if ((void*)shmTemp == (void*)-1) {
        perror("shmat");
        pthread_exit(NULL);
    }

    // Simulate reading and sending data in blocks
    int num_blocks = request.stop-request.start;

    for (int block = 0; block < num_blocks; block++) {

        char data_block[BLOCK_SIZE];
        sprintf(data_block, "This is block of file\n");
        
        sem_wait(&shmTemp->mutex);
        memcpy(shmTemp, data_block, BLOCK_SIZE);
        sem_post(&shmTemp->mutex);

        // Sleep or perform other operations between blocks if needed
    }

    // Detach from shared memory segment
    shmdt(shmTemp);

    pthread_exit(NULL);
}