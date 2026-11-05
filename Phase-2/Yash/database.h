#ifndef DATABASE_H
#define DATABASE_H

#include "passenger.h"

int initializeDatabase(void);

int savePassenger(const struct Passenger *p);

int loadPassengers(struct Passenger passengers[], int maxPassengers);

#endif
