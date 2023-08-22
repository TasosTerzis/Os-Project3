#include "./include/mutual.h"

int main(int argc, char** argv) {
    if (argc != 5) {
        printf("Usage: %s <N> <K> <L> <l>\n", argv[0]); exit(1); }

    int N = atoi(argv[1]); // N customers customers
    int K = atoi(argv[2]); // K available files in system
    int L = atoi(argv[3]); // L requests per customers
    int l = atoi(argv[4]); // l: Exponential time between SharedMemory

    // Create shared memory segment
    int shmid = shmget(IPC_PRIVATE, sizeof(SharedMemory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget"); exit(1);}

    // Attach shared memory segment
    SharedMemory shm = (SharedMemory)shmat(shmid, NULL, 0);
    if (shm ==(SharedMemory)(-1)) {
        perror("shmat"); exit(1);}

    // initialize shared memory
    // shm->queue = createQueue();
    shm->test = 10;

    // Initialize shm->queueSem semaphore
    // if (sem_init(&shm->queueSem, 1, 1) == -1) {
    //     perror("sem_init"); exit(1);}
    // if (sem_init(&shm->a, 1, 0) == -1) {
    //     perror("sem_init"); exit(1);}


    // fork N child customerss
    pid_t* customers = (pid_t *)malloc(N * sizeof(pid_t));
    if (customers == NULL) {
        perror("malloc"); return 1; }

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == 0) {     // Child process   
            customer(shm, K, L, l);
            exit(0);
        } 
        else if (pid > 0)  
            customers[i] = pid;
        else {
            perror("fork"); exit(1); }
    }

    // Your simulation logic goes here
    /* while (1){
        get the head of the fifo queue. 
        if (head == NULL){
            if(completed_requests == total_requests) {
                break;
            }
            else {
                continue;
                or 
                wait(semaphore) wait on a semaphore that gets posted when a request arrives
                that logic might be implied instead of all this block of code.
            }
        }
        else 
        {
            create thread to serve
        }

    } */

    // Use sem_wait and sem_post to control access to shared memory

    // wait for all child customers to complete
    for (int i = 0; i < K; i++) 
        waitpid(customers[i], NULL, 0);
    free(customers);


    // print queue
    printf("TEST: %d\n", shm->test);
    // printf("QUEUE SIZE: %d\n", queueSize(shm->queue));
    // printQueue(shm->queue);
    // destroyQueue(shm->queue);

    // Detach shared memory segment
    shmdt(shm);

    // Clean up shared memory and semaphore after the simulation
    shmctl(shmid, IPC_RMID, NULL);
    
    // sem_destroy();
    // sem_destroy();

    return 0;
}
