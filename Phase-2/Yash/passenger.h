#ifndef PASSENGER_H
#define PASSENGER_H

struct Passenger
{
    int id;
    char name[50];
    int age;
    int priority;
    int seatNo;
    int status;

    int coachType;
    float ticketPrice;
};

void inputPassenger(struct Passenger *p);
void displayPassenger(struct Passenger p);
void displayAllPassengers(struct Passenger passengers[], int count);
#endif