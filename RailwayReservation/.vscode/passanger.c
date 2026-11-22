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

    printf("Enter Priority (0 = Normal, 1 = Priority): ");
    scanf("%d", &p->priority);

    p->seatNo = -1;
    p->status = 0;
}

void displayPassenger(struct Passenger p)
{
    printf("\n--- Passenger Details ---\n");
    printf("ID       : %d\n", p.id);
    printf("Name     : %s\n", p.name);
    printf("Age      : %d\n", p.age);
    printf("Priority : %d\n", p.priority);
    printf("Seat No  : %d\n", p.seatNo);

    if (p.status == 1)
        printf("Status   : Confirmed\n");
    else
        printf("Status   : Waiting\n");
}