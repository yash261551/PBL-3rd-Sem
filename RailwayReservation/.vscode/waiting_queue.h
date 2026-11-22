#ifndef WAITING_QUEUE_H
#define WAITING_QUEUE_H

#include "passenger.h"

#define MAX_WAITING 20

extern struct Passenger waitingQueue[MAX_WAITING];
extern int front;
extern int rear;

void initializeWaitingQueue();
int isWaitingEmpty();
int isWaitingFull();
void enqueueWaiting(struct Passenger p);
struct Passenger dequeueWaiting();
void displayWaitingQueue();

#endif