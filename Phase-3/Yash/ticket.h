#ifndef TICKET_H
#define TICKET_H

#include "passenger.h"

#ifdef __cplusplus
extern "C" {
#endif

int generateTicket(const struct Passenger *p);

#ifdef __cplusplus
}
#endif

#endif