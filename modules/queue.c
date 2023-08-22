#include "../include/mutual.h"

// Function to create an empty queue
Queue createQueue() {
    Queue queue = (Queue)malloc(sizeof(struct queue));
    queue->front = queue->rear = NULL;
    queue->size = 0;
    return queue;
}

// Function to enqueue a request to the rear of the queue
void enqueue(Queue queue, Request request) {
    if (queue->rear == NULL) {
        queue->front = queue->rear = request;
        queue->size++;
        return;
    }
    queue->rear->next = request;
    queue->rear = request;
    queue->size++;
    gettimeofday(&request->requestTime, NULL);      // set request time since
}

// Function to dequeue a request from the front of the queue
void dequeue(Queue queue) {
    if (queue->front == NULL)
        return;

    Request temp = queue->front;
    queue->front = queue->front->next;

    if (queue->front == NULL)
        queue->rear = NULL;
    queue->size--;

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

int queueSize(Queue queue) {
    return queue->size;
}

void destroyQueue(Queue queue) {
    Request current = queue->front;
    while (current != NULL) {
        Request temp = current;
        current = current->next;
        free(temp);
    }
    free(queue);
}