#include <stdio.h>
#include "passenger.h"

void inputPassenger(struct Passenger *p)
{
    printf("\nEnter Passenger ID: ");
    scanf("%d", &p->id);

    printf("Enter Passenger Name: ");
    scanf(" %[^\n]", p->name);

    printf("Enter Age: ");
    scanf("%d", &p->age);

    printf("Enter Priority: ");
    scanf("%d", &p->priority);

    p->seatNo = -1;
    p->status = 0;
}

void displayPassenger(struct Passenger p)
{
    printf("\nPassenger ID   : %d", p.id);
    printf("\nName           : %s", p.name);
    printf("\nAge            : %d", p.age);
    printf("\nPriority       : %d", p.priority);
    printf("\nSeat Number    : %d", p.seatNo);

    if(p.status == 1)
        printf("\nStatus         : Confirmed\n");
    else if(p.status == 2)
        printf("\nStatus         : Waiting\n");
    else
        printf("\nStatus         : Cancelled\n");
}