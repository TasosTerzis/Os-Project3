#include "./include/mutual.h"

#define SHM_KEY 1234 // Shared memory key
#define SEM_KEY 5678 // Semaphore key

// Define shared memory structure
struct SharedMemory {
    // Add more fields to store requests and other data
};

// Function to initialize semaphores
int init_semaphores(int nsems) {
    int semid = semget(SEM_KEY, nsems, IPC_CREAT | 0666);
    // Initialize your semaphores here
    return semid;
}
int main (int argc, char** argv) {

    //collect command line arguments
    if(argc != 5) {
        printf("Usage: %s <N> <K> <L> <l>\n", argv[0]); exit(1); }
    int N = atoi(argv[1]);  // N customer processes
    int K = atoi(argv[2]);  // K available files in system
    int L = atoi(argv[3]);  // L requests per customer
    int l = atoi(argv[4]);  // l: Among the requests there is an exponentially distributed time


    int shmid;
    struct SharedMemory *shm_ptr;

    // Create shared memory segment
    shmid = shmget(SHM_KEY, sizeof(struct SharedMemory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory segment
    shm_ptr = (struct SharedMemory *)shmat(shmid, NULL, 0);
    if (shm_ptr == (struct SharedMemory *)(-1)) {
        perror("shmat");
        exit(1);
    }

    // Initialize semaphores
    int semid = init_semaphores(1); // You can adjust the number of semaphores

    // Your simulation logic goes here
    // Remember to synchronize access to shared memory using semaphores

    // Detach shared memory segment
    shmdt(shm_ptr);

    // Clean up shared memory and semaphores after the simulation
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;

}
