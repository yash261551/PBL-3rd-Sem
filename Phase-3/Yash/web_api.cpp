#include <cstring>
#include <cstdio>

#include "web_api.h"

extern "C" {
#include "database.h"
#include "booking.h"
#include "cancellation.h"
#include "waiting_queue.h"
#include "priority_queue.h"
#include "rac_queue.h"
#include "tilak_reservation.h"
#include "coach_management.h"
#include "ticket.h"
}

/* Maximum passengers supported */
#define WEB_MAX_PASSENGERS 100

/* Persistent backend data */
static struct Passenger passengers[WEB_MAX_PASSENGERS];

static int passengerCount = 0;
static int initialized = 0;


/* Find passenger using ID */
static int findPassengerIndex(int id)
{
    for (int i = 0; i < passengerCount; i++)
    {
        if (passengers[i].id == id)
            return i;
    }

    return -1;
}


/* Restore seats and queues from SQLite */
static void restoreData()
{
    initializeCoaches();

    initializeWaitingQueue();
    initializePriorityQueue();
    initializeRACQueue();

    for (int i = 0; i < passengerCount; i++)
    {
        struct Passenger p = passengers[i];

        if (p.status == STATUS_CONFIRMED)
        {
            if (p.seatNo > 0)
            {
                restoreSeatInCoach(
                    p.coachType,
                    p.seatNo
                );
            }
        }
        else if (p.status == STATUS_RAC)
        {
            enqueueRAC(p);
        }
        else if (p.status == STATUS_WAITING)
        {
            if (p.priority == 1)
                insertPriority(p);
            else
                enqueueWaiting(p);
        }
    }
}


/* Initialize backend */
extern "C" __declspec(dllexport)
int web_initialize(void)
{
    if (initialized)
        return 1;

    if (!initializeDatabase())
        return 0;

    passengerCount = loadPassengers(
        passengers,
        WEB_MAX_PASSENGERS
    );

    if (passengerCount < 0)
        return 0;

    restoreData();

    initialized = 1;

    return 1;
}


/* Register passenger */
extern "C" __declspec(dllexport)
int web_register_passenger(
    int id,
    const char *name,
    int age,
    int priority,
    int coachType
)
{
    if (!initialized || name == nullptr)
        return 0;

    if (id <= 0 ||
        age <= 0 || age > 120 ||
        (priority != 1 && priority != 2) ||
        coachType < 0 || coachType >= MAX_COACHES)
    {
        return 0;
    }

    if (passengerCount >= WEB_MAX_PASSENGERS)
        return 0;

    if (findPassengerIndex(id) != -1)
        return 0;

    struct Passenger p = {};

    p.id = id;

    std::strncpy(
        p.name,
        name,
        sizeof(p.name) - 1
    );

    p.age = age;
    p.priority = priority;
    p.coachType = coachType;

    p.pnr = 0;
    p.seatNo = -1;

    p.status = STATUS_CANCELLED;

    p.paymentStatus = PAYMENT_UNPAID;
    p.paymentMethod = PAYMENT_METHOD_NONE;

    p.ticketPrice = getCoachPrice(coachType);

    if (p.ticketPrice < 0)
        return 0;

    if (!savePassenger(&p))
        return 0;

    passengers[passengerCount++] = p;

    return 1;
}


/* Book ticket */
extern "C" __declspec(dllexport)
int web_book_ticket(
    int passengerID,
    int paymentMethod
)
{
    if (!initialized)
        return 0;

    int index = findPassengerIndex(passengerID);

    if (index == -1)
        return 0;

    struct Passenger *p = &passengers[index];

    if (p->pnr > 0)
        return 0;

    if (paymentMethod < PAYMENT_METHOD_CASH ||
        paymentMethod > PAYMENT_METHOD_NET_BANKING)
    {
        return 0;
    }

    p->pnr = generatePNR();

    if (p->pnr <= 0)
        return 0;

    float basePrice = getCoachPrice(p->coachType);

    p->ticketPrice = calculateTicketPrice(
        basePrice,
        p->priority
    );

    /*
       Existing reservation engine:
       Confirmed -> RAC -> Priority Waiting -> Normal Waiting
    */

    processReservation(p);

    if (p->status != STATUS_CONFIRMED &&
        p->status != STATUS_RAC &&
        p->status != STATUS_WAITING)
    {
        return 0;
    }

    /*
       Waiting-list passengers do not pay yet.
       Cash payments remain pending.
       Other web payment methods are simulated.
    */

    if (p->status == STATUS_WAITING)
    {
        p->paymentStatus = PAYMENT_UNPAID;
        p->paymentMethod = PAYMENT_METHOD_NONE;
    }
    else
    {
        p->paymentMethod = paymentMethod;

        if (paymentMethod == PAYMENT_METHOD_CASH)
            p->paymentStatus = PAYMENT_UNPAID;
        else
            p->paymentStatus = PAYMENT_PAID;
    }

    if (!savePassenger(p))
        return 0;

    recordBookingHistory(
        p,
        p->status == STATUS_CONFIRMED
            ? "WEB_BOOKED_CONFIRMED"
            : p->status == STATUS_RAC
                ? "WEB_BOOKED_RAC"
                : "WEB_BOOKED_WAITING"
    );

    return 1;
}


/* Promote next passenger after cancellation */
static void promoteNextPassenger(int coachType)
{
    struct Passenger next = {};

    next.id = -1;

    /* RAC receives the first opportunity */
    next = dequeueRACForCoach(coachType);

    /* Then priority waiting list */
    if (next.id <= 0)
        next = removePriorityForCoach(coachType);

    /* Finally normal waiting list */
    if (next.id <= 0)
        next = dequeueWaitingForCoach(coachType);

    if (next.id <= 0)
        return;

    int index = findPassengerIndex(next.id);

    if (index == -1)
        return;

    int seat = allocateSeatInCoach(coachType);

    if (seat == -1)
    {
        if (next.status == STATUS_RAC)
            enqueueRAC(next);
        else if (next.priority == 1)
            insertPriority(next);
        else
            enqueueWaiting(next);

        return;
    }

    next.seatNo = seat;
    next.status = STATUS_CONFIRMED;

    passengers[index] = next;

    savePassenger(&passengers[index]);

    recordBookingHistory(
        &passengers[index],
        "WEB_PROMOTED_TO_CONFIRMED"
    );
}


/* Cancel ticket */
extern "C" __declspec(dllexport)
int web_cancel_ticket(
    long long pnr,
    double *refundAmount
)
{
    if (!initialized || pnr <= 0)
        return 0;

    if (refundAmount != nullptr)
        *refundAmount = 0.0;

    struct Passenger p = {};

    if (!findPassengerByPNR(pnr, &p))
        return 0;

    if (p.status == STATUS_CANCELLED)
        return 0;

    int oldStatus = p.status;
    int coachType = p.coachType;

    if (oldStatus == STATUS_CONFIRMED)
    {
        releaseSeatInCoach(
            coachType,
            p.seatNo
        );
    }
    else if (oldStatus == STATUS_RAC)
    {
        removeRACPassenger(p.id);
    }
    else if (oldStatus == STATUS_WAITING)
    {
        if (p.priority == 1)
            removePriorityPassenger(p.id);
        else
            removeWaitingPassenger(p.id);
    }

    /* Simulated refund: 10% cancellation fee */
    if (p.paymentStatus == PAYMENT_PAID)
    {
        double refund = p.ticketPrice * 0.90;

        if (refundAmount != nullptr)
            *refundAmount = refund;
    }

    p.status = STATUS_CANCELLED;
    p.seatNo = -1;

    if (!savePassenger(&p))
        return 0;

    int index = findPassengerIndex(p.id);

    if (index != -1)
        passengers[index] = p;

    recordBookingHistory(
        &p,
        "WEB_CANCELLED"
    );

    /* Only confirmed-seat cancellation promotes someone */
    if (oldStatus == STATUS_CONFIRMED)
        promoteNextPassenger(coachType);

    return 1;
}


/* Get passenger count */
extern "C" __declspec(dllexport)
int web_get_passenger_count(void)
{
    return passengerCount;
}


/* Get passenger by array index */
extern "C" __declspec(dllexport)
int web_get_passenger(
    int index,
    struct Passenger *p
)
{
    if (!initialized || p == nullptr ||
        index < 0 || index >= passengerCount)
    {
        return 0;
    }

    *p = passengers[index];

    return 1;
}


/* Search PNR */
extern "C" __declspec(dllexport)
int web_find_pnr(
    long long pnr,
    struct Passenger *p
)
{
    if (!initialized || p == nullptr)
        return 0;

    return findPassengerByPNR(pnr, p);
}


/* Coach availability */
extern "C" __declspec(dllexport)
int web_get_coach_info(
    int coachType,
    int *totalSeats,
    int *bookedSeats,
    int *availableSeats,
    float *price
)
{
    if (!initialized ||
        coachType < 0 ||
        coachType >= MAX_COACHES)
    {
        return 0;
    }

    const int seatCapacity[MAX_COACHES] = {
        10, 20, 30, 40, 20
    };

    int booked = 0;

    for (int i = 0; i < passengerCount; i++)
    {
        if (passengers[i].coachType == coachType &&
            passengers[i].status == STATUS_CONFIRMED)
        {
            booked++;
        }
    }

    if (totalSeats)
        *totalSeats = seatCapacity[coachType];

    if (bookedSeats)
        *bookedSeats = booked;

    if (availableSeats)
        *availableSeats =
            seatCapacity[coachType] - booked;

    if (price)
        *price = getCoachPrice(coachType);

    return 1;
}


/* Count passengers by reservation status */
extern "C" __declspec(dllexport)
int web_get_queue_count(int status)
{
    int count = 0;

    for (int i = 0; i < passengerCount; i++)
    {
        if (passengers[i].status == status &&
            passengers[i].pnr > 0)
        {
            count++;
        }
    }

    return count;
}


/* Get passenger by reservation status and position */
extern "C" __declspec(dllexport)
int web_get_queue_passenger(
    int status,
    int position,
    struct Passenger *p
)
{
    if (!initialized || p == nullptr || position < 0)
        return 0;

    int current = 0;

    for (int i = 0; i < passengerCount; i++)
    {
        if (passengers[i].status == status &&
            passengers[i].pnr > 0)
        {
            if (current == position)
            {
                *p = passengers[i];
                return 1;
            }

            current++;
        }
    }

    return 0;
}


/* Generate ticket */
extern "C" __declspec(dllexport)
int web_generate_ticket(long long pnr)
{
    if (!initialized || pnr <= 0)
        return 0;

    struct Passenger p = {};

    if (!findPassengerByPNR(pnr, &p))
        return 0;

    return generateTicket(&p);
}