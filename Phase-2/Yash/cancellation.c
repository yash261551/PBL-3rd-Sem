#include <stdio.h>
#include "passenger.h"
#include "coach_management.h"
#include "tilak_reservation.h"

void cancelTicket(struct Passenger *p)
{
    if (p->status != 1)
    {
        printf("\nPassenger does not have a confirmed ticket.\n");
        return;
    }

    releaseSeatInCoach(p->coachType, p->seatNo);

    printf("\nTicket cancelled successfully.\n");
    printf("Passenger ID: %d\n", p->id);

    p->seatNo = -1;
    p->status = 0;
    p->ticketPrice = 0;

    moveWaitingPassengerToConfirmed();
}