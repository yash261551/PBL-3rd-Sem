#include <stdio.h>
#include "booking.h"
#include "seat_management.h"

void bookTicket(struct Passenger *p)
{
    int seat;

    seat = findAvailableSeat();

    if (seat == -1)
    {
        printf("\nNo seat available.\n");
        printf("Passenger cannot be confirmed.\n");
        return;
    }

    allocateSeat(seat);

    p->seatNo = seat;
    p->status = 1;

    printf("\n===== BOOKING CONFIRMED =====\n");
    printf("Passenger ID : %d\n", p->id);
    printf("Name         : %s\n", p->name);
    printf("Seat Number  : %d\n", p->seatNo);
}