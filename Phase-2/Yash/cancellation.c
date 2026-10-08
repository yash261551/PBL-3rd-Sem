#include <stdio.h>

#include "cancellation.h"
#include "seat_management.h"
#include "tilak_reservation.h"

void cancelTicket(struct Passenger *p)
{
    if(p->status != 1)
    {
        printf("\nPassenger does not have a confirmed ticket.\n");
        return;
    }

    releaseSeat(p->seatNo);

    p->seatNo = -1;
    p->status = 0;

    printf("\nTicket cancelled successfully.\n");
    printf("Passenger ID : %d\n", p->id);

    /* Give the released seat to waiting passenger */
    moveWaitingPassengerToConfirmed();
}