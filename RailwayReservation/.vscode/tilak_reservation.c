#include <stdio.h>

#include "tilak_reservation.h"
#include "waiting_queue.h"
#include "priority_queue.h"
#include "seat_management.h"

void processReservation(struct Passenger *p)
{
    int seat;

    seat = findAvailableSeat();

    if (seat != -1)
    {
        allocateSeat(seat);

        p->seatNo = seat;
        p->status = 1;

        printf("\nBooking Confirmed!\n");
        printf("Passenger: %s\n", p->name);
        printf("Seat Number: %d\n", p->seatNo);
    }
    else
    {
        p->seatNo = -1;
        p->status = 0;

        if (p->priority > 0)
        {
            insertPriority(*p);
        }
        else
        {
            enqueueWaiting(*p);
        }

        printf("\nNo seat available.\n");
        printf("Passenger added to waiting list.\n");
    }
}

void moveWaitingPassengerToConfirmed()
{
    struct Passenger p;
    int seat;

    seat = findAvailableSeat();

    if (seat == -1)
    {
        printf("No free seat available.\n");
        return;
    }

    if (!isPriorityEmpty())
    {
        p = removePriority();
    }
    else if (!isWaitingEmpty())
    {
        p = dequeueWaiting();
    }
    else
    {
        printf("No passenger is waiting.\n");
        return;
    }

    allocateSeat(seat);

    p.seatNo = seat;
    p.status = 1;

    printf("\nWaiting passenger promoted to confirmed booking.\n");
    printf("Passenger: %s\n", p.name);
    printf("Seat Number: %d\n", p.seatNo);
}