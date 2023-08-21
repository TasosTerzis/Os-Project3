#include "./include/mutual.h"

#define NUM_FILES 5  // Number of available files
#define SHM_KEY 1234 // Shared memory key


int main(int argc, char** argv) {
    if (argc != 5) {
        printf("Usage: %s <N> <K> <L> <l>\n", argv[0]);
        exit(1);
    }

    int N = atoi(argv[1]); // N customer processes
    int K = atoi(argv[2]); // K available files in system
    int L = atoi(argv[3]); // L SharedMemory per customer
    int l = atoi(argv[4]); // l: Exponential time between SharedMemory

    // Create shared memory segment
    int shmid = shmget(IPC_PRIVATE, sizeof(SharedMemory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget"); exit(1);}

    // Attach shared memory segment
    SharedMemory shm = (SharedMemory)shmat(shmid, NULL, 0);
    if (shm ==(SharedMemory)(-1)) {
        perror("shmat"); exit(1);}

    // Initialize POSIX semaphore
    sem_t semaphore;
    if (sem_init(&semaphore, 1, 1) == -1) {
        perror("sem_init");
        exit(1);
    }

    // Your simulation logic goes here
    // while (completed_requests <= total_requests){
        // get the head of the fifo queue. 
        // if (head == NULL)
        //      wait
        // else 
        // {
            //create thread to serve
        //}

    //}

    // Use sem_wait and sem_post to control access to shared memory

    // Detach shared memory segment
    shmdt(shm);

    // Clean up shared memory and semaphore after the simulation
    shmctl(shmid, IPC_RMID, NULL);
    sem_destroy(&semaphore);

    return 0;
}
