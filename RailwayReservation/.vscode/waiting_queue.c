#include <stdio.h>
#include "waiting_queue.h"

struct Passenger waitingQueue[MAX_WAITING];

int front = -1;
int rear = -1;

void initializeWaitingQueue()
{
    front = -1;
    rear = -1;
}

int isWaitingEmpty()
{
    return front == -1;
}

int isWaitingFull()
{
    return rear == MAX_WAITING - 1;
}

void enqueueWaiting(struct Passenger p)
{
    if (isWaitingFull())
    {
        printf("Waiting list is full.\n");
        return;
    }

    if (front == -1)
    {
        front = 0;
    }

    rear++;
    waitingQueue[rear] = p;

    printf("Passenger added to normal waiting list.\n");
}

struct Passenger dequeueWaiting()
{
    struct Passenger p;

    if (isWaitingEmpty())
    {
        p.id = -1;
        return p;
    }

    p = waitingQueue[front];

    if (front == rear)
    {
        front = -1;
        rear = -1;
    }
    else
    {
        front++;
    }

    return p;
}

void displayWaitingQueue()
{
    int i;

    if (isWaitingEmpty())
    {
        printf("Normal waiting list is empty.\n");
        return;
    }

    printf("\n--- Normal Waiting List ---\n");

    for (i = front; i <= rear; i++)
    {
        printf("ID: %d | Name: %s | Age: %d\n",
               waitingQueue[i].id,
               waitingQueue[i].name,
               waitingQueue[i].age);
    }
}