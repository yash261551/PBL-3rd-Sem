#include <stdio.h>
#include "coach_management.h"
#include "database.h"
#include "seat_management.h"
#include "passenger.h"
#include "booking.h"
#include "cancellation.h"
#include "waiting_queue.h"
#include "priority_queue.h"
#define MAX_PASSENGERS 100
int main()
{
    struct Passenger passengers[MAX_PASSENGERS];
    int count = 0;

    if (!initializeDatabase())
    {
        printf("Failed to initialize database.\n");
        return 1;
    }

    count = loadPassengers(passengers, MAX_PASSENGERS);

    initializeCoaches();

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
        printf("\n4. Display All Passengers");
        printf("\n5. Display Coach Availability");
        printf("\n6. Display Ticket Prices");
        printf("\n7. Exit");

        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:

                inputPassenger(&passengers[count]);
                if (savePassenger(&passengers[count]))
                {
                    count++;
                    printf("\nPassenger added successfully!\n");
                }
                else
                {
                    printf("\nFailed to save passenger. Please try again.\n");
                }
                break;


            case 2:

                if (count > 0)
                {
                    bookTicket(&passengers[count - 1]);
                    savePassenger(&passengers[count - 1]);
                }
                else
                {
                    printf("Please add a passenger first.\n");
                }

                break;


            case 3:

                if (count > 0)
                {
                    cancelTicket(&passengers[count - 1]);
                    savePassenger(&passengers[count - 1]);
                }
                else
                {
                    printf("Please add a passenger first.\n");
                }
                break;


            case 4:
                displayAllPassengers(passengers, count);
                break;


            case 5:
                displayCoachAvailability();
                break;

            case 6:
                displayCoaches();
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