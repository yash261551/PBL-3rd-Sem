#include <stdio.h>

#include "seat_management.h"
#include "passenger.h"
#include "booking.h"
#include "cancellation.h"
#include "waiting_queue.h"
#include "priority_queue.h"

int main()
{
    struct Passenger p;

    int choice;
    int passengerCreated = 0;

    initializeSeats();
    initializeWaitingQueue();

    do
    {
        printf("\n\n====================================");
        printf("\n   RAILWAY RESERVATION SYSTEM");
        printf("\n====================================");

        printf("\n1. Add Passenger");
        printf("\n2. Book Ticket");
        printf("\n3. Cancel Ticket");
        printf("\n4. Display Passenger");
        printf("\n5. Display Seats");
        printf("\n6. Display Waiting List");
        printf("\n7. Display Priority Waiting List");
        printf("\n8. Exit");

        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:

                inputPassenger(&p);

                passengerCreated = 1;

                printf("\nPassenger added successfully.");

                break;


            case 2:

                if(passengerCreated == 0)
                {
                    printf("\nPlease add passenger first.");
                }
                else
                {
                    bookTicket(&p);
                }

                break;


            case 3:

                if(passengerCreated == 0)
                {
                    printf("\nNo passenger available.");
                }
                else
                {
                    cancelTicket(&p);
                }

                break;


            case 4:

                if(passengerCreated == 0)
                {
                    printf("\nNo passenger available.");
                }
                else
                {
                    displayPassenger(p);
                }

                break;


            case 5:

                displaySeats();

                break;


            case 6:

                displayWaitingQueue();

                break;


            case 7:

                displayPriorityQueue();

                break;


            case 8:

                printf("\nExiting Railway Reservation System...\n");

                break;


            default:

                printf("\nInvalid choice.");
        }

    } while(choice != 8);

    return 0;
}