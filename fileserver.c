#include "./include/mutual.h"

int main(int argc, char** argv) {
    if (argc != 5) {
        printf("Usage: %s <N> <K> <L> <l>\n", argv[0]); exit(1); }

    int N = atoi(argv[1]); // N customers customers
    int K = atoi(argv[2]); // K available files in system
    int L = atoi(argv[3]); // L requests per customers
    float l = atof(argv[4]); // l: Exponential time between SharedMemory
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
    shm->in = shm->out = shm->size = 0;

    // Initialize shm->queueSem semaphore
    if (sem_init(&shm->mutex, 1, 1) == -1) {
        perror("sem_init"); exit(1);}
    if (sem_init(&shm->empty, 1, MAX_QUEUE_SIZE) == -1) {
        perror("sem_init"); exit(1);}
    if (sem_init(&shm->full, 1, 0) == -1) {
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
    int count=0;
    while(1) {

        sem_wait(&shm->full);
        sem_wait(&shm->mutex);

        // Get request from shared memory
        Request request = shm->array[shm->out];
        shm->out = (shm->out + 1) % MAX_QUEUE_SIZE;
        shm->size--;
        count++;
        if(count == N*L) {
            sem_post(&shm->mutex);
            sem_post(&shm->full);
            break;
        }
        sem_post(&shm->mutex);
        sem_post(&shm->empty);

    }

    // wait for all child customers to finish
    for (int i = 0; i < N; i++) 
        waitpid(customers[i], NULL, 0);
    free(customers);

    // Detach shared memory segment
    shmdt(shm);

    // Clean up shared memory and semaphore after the simulation
    shmctl(shmid, IPC_RMID, NULL);
    
    // destroy semaphores
    sem_destroy(&shm->mutex);
    sem_destroy(&shm->empty);
    sem_destroy(&shm->full);


    return 0;
}
