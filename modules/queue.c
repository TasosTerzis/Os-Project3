#include "../include/mutual.h"

// Function to enqueue a request to the rear of the queue
void enqueue(Queue* queue, Request request) {
    if (queue->size < MAX_QUEUE_SIZE) {
        queue->array[queue->rear] = request;
        queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE;
        queue->size++;
        gettimeofday(&queue->array[queue->rear - 1].requestTime, NULL); // set request time since
    } 
}

// Function to dequeue a request from the front of the queue
Request dequeue(Queue* queue) {
    Request request;
    if (queue->size > 0) {
        request = queue->array[queue->front];
        queue->front = (queue->front + 1) % MAX_QUEUE_SIZE;
        queue->size--;
    }
    return request;
}

// Function to print the requests in the queue
void printQueue(Queue* queue) {
    // int currentIdx = queue->front;
    // for (int i = 0; i < queue->size; i++) {
    //     printf("PID: %d, FileNum: %d\n", queue->array[currentIdx].pid, queue->array[currentIdx].fileNum);
    //     currentIdx = (currentIdx + 1) % MAX_QUEUE_SIZE;
    // }
    // printf("\n");
}

int queueSize(Queue* queue) {
    return queue->size;
}