#include <stdio.h>
#include "cancellation.h"
#include "seat_management.h"

void cancelTicket(struct Passenger *p)
{
    if (p->status != 1)
    {
        printf("\nNo confirmed ticket found for this passenger.\n");
        return;
    }

    if (p->seatNo == -1)
    {
        printf("\nInvalid seat number.\n");
        return;
    }

    releaseSeat(p->seatNo);

    printf("\n===== TICKET CANCELLED =====\n");
    printf("Passenger ID : %d\n", p->id);
    printf("Name         : %s\n", p->name);
    printf("Seat Number  : %d\n", p->seatNo);

    p->seatNo = -1;
    p->status = 0;

    printf("Seat is now available.\n");
}