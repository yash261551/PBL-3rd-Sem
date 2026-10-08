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
};

void inputPassenger(struct Passenger *p);
void displayPassenger(struct Passenger p);

#endif
