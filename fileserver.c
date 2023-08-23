#include "./include/mutual.h"

int main(int argc, char** argv) {
    if (argc != 5) {
        printf("Usage: %s <N> <K> <L> <l>\n", argv[0]); exit(1); }

    int N = atoi(argv[1]); // N customers customers
    int K = atoi(argv[2]); // K available files in system
    int L = atoi(argv[3]); // L requests per customers
    int l = atoi(argv[4]); // l: Exponential time between SharedMemory
    if(L > MAX_QUEUE_SIZE) {
        printf("L must be <= MAX_QUEUE_SIZE: %d\n", MAX_QUEUE_SIZE); exit(1); }

    // Create shared memory segment
    int shmid = shmget(IPC_PRIVATE, sizeof(SharedMemory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget"); exit(1);}

    // Attach shared memory segment
    SharedMemory shm = (SharedMemory)shmat(shmid, NULL, 0);
    if (shm ==(SharedMemory)(-1)) {
        perror("shmat"); exit(1);}

    // initialize shared memory
    shm->queue.front = shm->queue.rear = shm->queue.size = 0;


    // Initialize shm->queueSem semaphore
    if (sem_init(&shm->queueSem, 1, 1) == -1) {
        perror("sem_init"); exit(1);}
    if (sem_init(&shm->nonEmpty, 1, 0) == -1) {
        perror("sem_init"); exit(1);}
    if (sem_init(&shm->countSem, 1, 1) == -1) {
        perror("sem_init"); exit(1);}


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
    while(1) {

        sem_wait(&shm->nonEmpty); // wait if queue is currently empty
        sem_wait(&shm->countSem); 
        if(shm->count == N*L)
            break;
        sem_post(&shm->countSem);

        sem_wait(&shm->queueSem); // wait if queue is currently being accessed by another thread
        Request request = dequeue(&shm->queue);
        if(queueSize(&shm->queue) == 0)
            sem_post(&shm->nonEmpty); // wait if queue is currently empty
        sem_post(&shm->queueSem); // release queue for other threads to access
        printf("PID: %d, FileNum: %d\n", request.pid, request.fileNum);

    }
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



    // wait for all child customers to finish
    for (int i = 0; i < N; i++) {
        waitpid(customers[i], NULL, 0);
    }
    free(customers);


    // print queue
    printf("Queue size: %d\n", shm->queue.size);
    printQueue(&shm->queue);

    // Detach shared memory segment
    shmdt(shm);

    // Clean up shared memory and semaphore after the simulation
    shmctl(shmid, IPC_RMID, NULL);
    
    // destroy semaphores
    sem_destroy(&shm->queueSem);
    sem_destroy(&shm->nonEmpty);

    return 0;
}
