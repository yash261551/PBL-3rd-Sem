#include <stdio.h>
#include "seat_management.h"

int seats[MAX_SEATS];

void initializeSeats()
{
    int i;

    for(i = 0; i < MAX_SEATS; i++)
    {
        seats[i] = 0;
    }
}

void displaySeats()
{
    int i;

    printf("\n===== SEAT STATUS =====\n");

    for(i = 0; i < MAX_SEATS; i++)
    {
        if(seats[i] == 0)
            printf("Seat %d : Available\n", i + 1);
        else
            printf("Seat %d : Booked\n", i + 1);
    }
}

int findAvailableSeat()
{
    int i;

    for(i = 0; i < MAX_SEATS; i++)
    {
        if(seats[i] == 0)
            return i;
    }

    return -1;
}

int allocateSeat()
{
    int index;

    index = findAvailableSeat();

    if(index == -1)
        return -1;

    seats[index] = 1;

    return index + 1;
}

void releaseSeat(int seatNo)
{
    if(seatNo >= 1 && seatNo <= MAX_SEATS)
    {
        seats[seatNo - 1] = 0;
        printf("Seat %d released successfully.\n", seatNo);
    }
    else
    {
        printf("Invalid seat number.\n");
    }
}