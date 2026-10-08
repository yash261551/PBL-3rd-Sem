#ifndef SEAT_MANAGEMENT_H
#define SEAT_MANAGEMENT_H

#define MAX_SEATS 10

extern int seats[MAX_SEATS];

void initializeSeats();
void displaySeats();
int findAvailableSeat();
int allocateSeat();
void releaseSeat(int seatNo);

#endif