#include <stdio.h>
#include "priority_queue.h"

struct Passenger priorityQueue[MAX_PRIORITY];
int priorityCount = 0;

void insertPriority(struct Passenger p)
{
    int i;

    if (priorityCount == MAX_PRIORITY)
    {
        printf("Priority waiting list is full.\n");
        return;
    }

    i = priorityCount - 1;

    while (i >= 0 && priorityQueue[i].priority > p.priority)
    {
        priorityQueue[i + 1] = priorityQueue[i];
        i--;
    }

    priorityQueue[i + 1] = p;
    priorityCount++;

    printf("Passenger added to priority waiting list.\n");
}

struct Passenger removePriority()
{
    struct Passenger p;
    int i;

    if (priorityCount == 0)
    {
        p.id = -1;
        return p;
    }

    p = priorityQueue[0];

    for (i = 1; i < priorityCount; i++)
    {
        priorityQueue[i - 1] = priorityQueue[i];
    }

    priorityCount--;

    return p;
}

int isPriorityEmpty()
{
    return priorityCount == 0;
}

void displayPriorityQueue()
{
    int i;

    if (priorityCount == 0)
    {
        printf("Priority waiting list is empty.\n");
        return;
    }

    printf("\n--- Priority Waiting List ---\n");

    for (i = 0; i < priorityCount; i++)
    {
        printf("ID: %d | Name: %s | Priority: %d\n",
               priorityQueue[i].id,
               priorityQueue[i].name,
               priorityQueue[i].priority);
    }
}