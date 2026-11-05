#include <stdio.h>
#include "booking.h"
#include "passenger.h"
#include "tilak_reservation.h"
#include "payment.h"
void bookTicket(struct Passenger *p)
{
    char paymentMethod[30];

    processReservation(p);

    if (p->status == 1)
    {
        if (acceptPayment(p->ticketPrice,
                          paymentMethod,
                          sizeof(paymentMethod)))
        {
            printf("\nTicket booked and payment completed.\n");
        }
        else
        {
            printf("\nPayment failed. Please contact the reservation counter.\n");
        }
    }
    else
    {
        printf("\nPassenger is on the waiting list.\n");
        printf("Payment has not been collected.\n");
    }
}
