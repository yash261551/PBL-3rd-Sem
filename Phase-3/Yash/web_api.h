#ifndef WEB_API_H
#define WEB_API_H

#include "passenger.h"

#ifdef __cplusplus
extern "C" {
#endif

int web_initialize(void);

int web_register_passenger(
    int id,
    const char *name,
    int age,
    int priority,
    int coachType
);

int web_book_ticket(int passengerID, int paymentMethod);

int web_cancel_ticket(long long pnr, double *refundAmount);

int web_get_passenger_count(void);

int web_get_passenger(int index, struct Passenger *p);

int web_find_pnr(long long pnr, struct Passenger *p);

int web_get_coach_info(
    int coachType,
    int *totalSeats,
    int *bookedSeats,
    int *availableSeats,
    float *price
);

int web_get_queue_count(int status);

int web_get_queue_passenger(
    int status,
    int index,
    struct Passenger *p
);

int web_generate_ticket(long long pnr);

#ifdef __cplusplus
}
#endif

#endif