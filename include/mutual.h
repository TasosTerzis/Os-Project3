#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>
#define TOTAL_FILES 10   // fileserver has exactly TOTAL_FILES files 
#define TOTAL_LINES 100  // each file has 20 exactly TOTAL_LINES lines
#define BLOCK_SIZE 100   // equal to MAX_LINE_LENGTH. (The existing files have about 70 characters per line.)


// Define REQUESTS structure
typedef struct request* Request;
struct request {
    pid_t pid; // client pid
    int fileNum;
    int start, stop;
    struct timeval requestTime; // time of request
    Request next;
};

// fifo dynamic queue to store requests
struct queue {
    Request front;
    Request rear;
    int size;
};
typedef struct queue* Queue;

struct shared_memory {
    int test;
    // sem_t queueSem;
    // sem_t a;   // semaphore for server to wait if queue is currently empty.
    // Queue queue;
                // probably i'll add a check for new request: if queueSize was 0, then increase semaphore a.
                // server had been waiting on a semaphore a, so it will be released and will check the queue again.
                // also, when a request is completed, request will check if it was the last one so. 
};
typedef struct shared_memory* SharedMemory;

void customer (SharedMemory, int K, int L, int l);

// Function to create an empty queue
Queue createQueue();

// Function to enqueue a request to the rear of the queue
void enqueue(Queue, Request);

// Function to dequeue a request from the front of the queue
void dequeue(Queue);

int queueSize(Queue queue);

// Function to print the requests in the queue
void printQueue(Queue);

//
void destroyQueue(Queue queue);