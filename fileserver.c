#include "./include/mutual.h"

sem_t fileSem[TOTAL_FILES];

int main(int argc, char** argv) {
    
    if (argc != 5) {
        printf("Usage: %s <N> <K> <L> <l>\n", argv[0]); exit(1); }

    int N = atoi(argv[1]); // N customers customers
    int K = atoi(argv[2]); // K available files in system
    int L = atoi(argv[3]); // L requests per customers
    float l = atof(argv[4]); // l: Exponential time between SharedMemory
    if(L > MAX_QUEUE_SIZE) {
        printf("L must be <= MAX_QUEUE_SIZE: %d\n", MAX_QUEUE_SIZE); exit(1); }

    // delete all log files
    system("rm -rf log/*");

    // initialize file semaphores
    for (int i = 0; i < TOTAL_FILES; i++) {
        char sem_name[20];
        sprintf(sem_name, "/file%d", i);
        if (sem_init(&fileSem[i], 1, 1) == -1) {
            perror("sem_init"); exit(1); }
    }

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
            customer(shm, K, L, l, i);
            exit(0);
        } 
        else if (pid > 0)  
            customers[i] = pid;
        else {
            perror("fork"); exit(1); }
    }


    // Your simulation logic goes here
    int count=0;
    // dynamic array to store thread ids
    pthread_t* thread_ids = (pthread_t *)malloc(N*L * sizeof(pthread_t));

    while(1) {

        sem_wait(&shm->full);
        sem_wait(&shm->mutex);

        // Get request from shared memory
        Request* request = malloc(sizeof(Request));
        *request = shm->array[shm->out];
        // printf("Ser-Loop-> Request: file%d, lines %d-%d, process %d\n", request->fileNum, request->start, request->stop, request->pid);

        // whenever server gets a request, it will create a new thread to handle it
        pthread_t tid;
        pthread_create(&tid, NULL, serverThread, request);
        thread_ids[count] = tid; // Store thread ID

        // free(request);
        shm->out = (shm->out + 1) % MAX_QUEUE_SIZE;
        shm->size--;
        count++;
        if(count == N*L) {
            sem_post(&shm->mutex);
            sem_post(&shm->empty);
            break;
        }
        sem_post(&shm->mutex);
        sem_post(&shm->empty);

    }

    // Wait for all threads to finish
    for (int i = 0; i < count; i++) 
        pthread_join(thread_ids[i], NULL);
    free(thread_ids);

    // wait for all child customers to finish
    for (int i = 0; i < N; i++) 
        waitpid(customers[i], NULL, 0);
    free(customers);

    // Detach shared memory segment
    shmdt(shm);
    
    // destroy semaphores
    for (int i = 0; i < TOTAL_FILES; i++) 
        sem_destroy(&fileSem[i]);
    sem_destroy(&shm->mutex);
    sem_destroy(&shm->empty);
    sem_destroy(&shm->full);



    return 0;
}
