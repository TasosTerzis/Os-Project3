#include "../include/mutual.h"

// Function to create a new request
Request createRequest(pid_t pid, int fileNum, int start, int stop, time_t requestTime, time_t completeTime) {
    Request newRequest = (Request)malloc(sizeof(struct request));
    newRequest->pid = pid;
    newRequest->fileNum = fileNum;
    newRequest->start = start;
    newRequest->stop = stop;
    newRequest->requestTime = requestTime;
    newRequest->completeTime = completeTime;
    newRequest->next = NULL;
    return newRequest;
}

// Function to create an empty queue
Queue createQueue() {
    Queue newQueue = (Queue)malloc(sizeof(struct queue));
    newQueue->front = newQueue->rear = NULL;
    return newQueue;
}

// Function to enqueue a request to the rear of the queue
void enqueue(Queue queue, pid_t pid, int fileNum, int start, int stop, time_t requestTime, time_t completeTime) {
    Request newRequest = createRequest(pid, fileNum, start, stop, requestTime, completeTime);
    if (queue->rear == NULL) {
        queue->front = queue->rear = newRequest;
        return;
    }
    queue->rear->next = newRequest;
    queue->rear = newRequest;
}

// Function to dequeue a request from the front of the queue
void dequeue(Queue queue) {
    if (queue->front == NULL)
        return;

    Request temp = queue->front;
    queue->front = queue->front->next;

    if (queue->front == NULL)
        queue->rear = NULL;

    free(temp);
}

// Function to print the requests in the queue
void printQueue(Queue queue) {
    Request current = queue->front;
    while (current != NULL) {
        printf("PID: %d, FileNum: %d\n", current->pid, current->fileNum);
        current = current->next;
    }
    printf("\n");
}