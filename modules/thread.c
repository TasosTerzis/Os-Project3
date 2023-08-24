#include "../include/mutual.h"

extern sem_t fileSem[TOTAL_FILES];

void* serverThread(void* arg) {

    Request request = *(Request*)arg;

    // Attach to the shared memory segment of the client process
    TempSharedMemory shmTemp = (TempSharedMemory)shmat(request.shmTempId, NULL, 0);
    if ((void*)shmTemp == (void*)-1) {
        perror("shmat"); pthread_exit(NULL); }

    // Simulate reading and sending data in blocks
    int num_blocks = request.stop-request.start;
    
    // open the file that is found in the files folder
    // sem_wait(&fileSem[request.fileNum]);
    char filename[20];
    sprintf(filename, "files/file%d.txt", request.fileNum);
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen"); pthread_exit(NULL); }

    int current_line = 0;
    char data_block[BLOCK_SIZE];

    while (current_line < request.start-1) {
        if (fgets(data_block, BLOCK_SIZE, file) == NULL) {
            perror("fgets"); pthread_exit(NULL); }
        current_line++;
    }

    // print request info
    printf("Request: file%d, lines %d-%d, process %d\n", request.fileNum, request.start, request.stop, request.pid);

    // for (int block = 0; block <= num_blocks; block++) {
        
    //     if(block>0){
    //         sem_wait(&shmTemp->dataEaten);
    //     }
    //     char* line = fgets(data_block, BLOCK_SIZE, file);
    //     if (line == NULL) {
    //         perror("fgets"); pthread_exit(NULL);}

    //     sem_wait(&shmTemp->mutex);
    //     memcpy(shmTemp->array, data_block, BLOCK_SIZE);
    //     sem_post(&shmTemp->mutex);
    //     sem_post(&shmTemp->dataReady);

    //     // Sleep or perform other operations between blocks if needed
    // }
    // sem_post(&fileSem[request.fileNum]);
    

    fclose(file);

    // Detach from shared memory segment
    shmdt(shmTemp);
    // shmctl(request.shmTempId, IPC_RMID, NULL);

    pthread_exit(NULL);
}