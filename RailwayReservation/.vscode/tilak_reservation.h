#ifndef TILAK_RESERVATION_H
#define TILAK_RESERVATION_H

#include "passenger.h"

void processReservation(struct Passenger *p);
void moveWaitingPassengerToConfirmed();

#endif
