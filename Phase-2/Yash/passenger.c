#include <stdio.h>
#include "passenger.h"
#include "coach_management.h"

void inputPassenger(struct Passenger *p)
{
    printf("\nEnter Passenger ID: ");
    scanf("%d", &p->id);

    printf("Enter Passenger Name: ");
    scanf(" %49[^\n]", p->name);

    printf("Enter Age: ");
    scanf("%d", &p->age);

    printf("Enter Priority (1 = Priority, 2 = Normal): ");
    scanf("%d", &p->priority);

    displayCoaches();

    do
    {
        printf("\nSelect Coach (1-5): ");
        scanf("%d", &p->coachType);
    }
    while (p->coachType < 1 || p->coachType > 5);

    p->coachType--;

    p->seatNo = -1;
    p->status = 0;
    p->ticketPrice = getCoachPrice(p->coachType);
}

void displayPassenger(struct Passenger p)
{
    printf("\nPassenger ID: %d", p.id);
    printf("\nName: %s", p.name);
    printf("\nAge: %d", p.age);
    printf("\nCoach: %s", getCoachName(p.coachType));
    printf("\nTicket Price: Rs. %.2f", p.ticketPrice);
    printf("\nSeat Number: %d", p.seatNo);

    if (p.status == 1)
    {
        printf("\nStatus: Confirmed\n");
    }
    else if (p.status == 2)
    {
        printf("\nStatus: Waiting\n");
    }
    else
    {
        printf("\nStatus: Not Booked\n");
    }
}
void displayAllPassengers(struct Passenger passengers[], int count)
{
    int i;

    if (count == 0)
    {
        printf("\nNo passengers available.\n");
        return;
    }

    printf("\n========== ALL PASSENGERS ==========\n");

    for (i = 0; i < count; i++)
    {
        displayPassenger(passengers[i]);
        printf("------------------------------------\n");
    }
}