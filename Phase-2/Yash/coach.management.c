#include <stdio.h>
#include "coach_management.h"

#define MAX_SEATS 40

struct Coach
{
    char name[30];
    int totalSeats;
    float price;
};

static struct Coach coaches[MAX_COACHES] =
{
    {"A1 - AC First Class", 10, 2000},
    {"B1 - AC 2 Tier", 20, 1500},
    {"C1 - AC 3 Tier", 30, 1000},
    {"S1 - Sleeper Class", 40, 600},
    {"D1 - Chair Car", 20, 400}
};

static int seats[MAX_COACHES][MAX_SEATS];

void initializeCoaches(void)
{
    int i, j;

    for (i = 0; i < MAX_COACHES; i++)
    {
        for (j = 0; j < MAX_SEATS; j++)
        {
            seats[i][j] = 0;
        }
    }
}

void displayCoaches(void)
{
    int i;

    printf("\n========== AVAILABLE COACHES ==========\n");

    for (i = 0; i < MAX_COACHES; i++)
    {
        printf("%d. %s | Fare: Rs. %.2f\n",
               i + 1, coaches[i].name, coaches[i].price);
    }
}

void displayCoachAvailability(void)
{
    int i, j, booked, available;

    printf("\n========== COACH AVAILABILITY ==========\n");

    for (i = 0; i < MAX_COACHES; i++)
    {
        booked = 0;

        for (j = 0; j < coaches[i].totalSeats; j++)
        {
            if (seats[i][j] == 1)
            {
                booked++;
            }
        }

        available = coaches[i].totalSeats - booked;

        printf("\n%s\n", coaches[i].name);
        printf("Total Seats: %d\n", coaches[i].totalSeats);
        printf("Booked Seats: %d\n", booked);
        printf("Available Seats: %d\n", available);
    }
}

float getCoachPrice(int coachType)
{
    if (coachType < 0 || coachType >= MAX_COACHES)
    {
        return -1;
    }

    return coaches[coachType].price;
}

const char *getCoachName(int coachType)
{
    if (coachType < 0 || coachType >= MAX_COACHES)
    {
        return "Invalid Coach";
    }

    return coaches[coachType].name;
}

int allocateSeatInCoach(int coachType)
{
    int i;

    if (coachType < 0 || coachType >= MAX_COACHES)
    {
        return -1;
    }

    for (i = 0; i < coaches[coachType].totalSeats; i++)
    {
        if (seats[coachType][i] == 0)
        {
            seats[coachType][i] = 1;
            return i + 1;
        }
    }

    return -1;
}

void releaseSeatInCoach(int coachType, int seatNo)
{
    if (coachType < 0 || coachType >= MAX_COACHES)
    {
        return;
    }

    if (seatNo < 1 || seatNo > coaches[coachType].totalSeats)
    {
        return;
    }

    seats[coachType][seatNo - 1] = 0;
}