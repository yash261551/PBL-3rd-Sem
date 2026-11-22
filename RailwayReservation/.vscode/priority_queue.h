#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "passenger.h"

#define MAX_PRIORITY 20

void insertPriority(struct Passenger p);

struct Passenger removePriority();

void displayPriorityQueue();

int isPriorityEmpty();

#endif