#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <string>

#include "ticket.h"
#include "coach_management.h"

using namespace std;

class Ticket
{
private:
    const Passenger *passenger;

public:
    Ticket(const Passenger *p) : passenger(p)
    {
    }

    string getStatus() const
    {
        switch (passenger->status)
        {
            case STATUS_CONFIRMED:
                return "CONFIRMED";

            case STATUS_RAC:
                return "RAC";

            case STATUS_WAITING:
                return "WAITING";

            case STATUS_CANCELLED:
                return "CANCELLED";

            default:
                return "UNKNOWN";
        }
    }

    void display(ostream &out) const
    {
        time_t now = time(nullptr);
        char dateTime[30] = "Unavailable";

        std::tm *currentTime = std::localtime(&now);

        if (currentTime != nullptr)
        {
            strftime(
                dateTime,
                sizeof(dateTime),
                "%d-%m-%Y %H:%M:%S",
                currentTime
            );
        }

        const char *coachName =
            getCoachName(passenger->coachType);

        out << "\n";
        out << "========================================\n";
        out << "       RAILWAY RESERVATION SYSTEM       \n";
        out << "              E-TICKET                 \n";
        out << "========================================\n";

        out << "\nPNR Number       : " << passenger->pnr;
        out << "\nPassenger ID     : " << passenger->id;
        out << "\nPassenger Name   : " << passenger->name;
        out << "\nAge              : " << passenger->age;

        out << "\nCoach            : "
            << (coachName != nullptr ? coachName : "Unknown");

        out << "\nSeat Number      : ";

        if (passenger->seatNo > 0)
            out << passenger->seatNo;
        else
            out << "Not Allocated";

        out << "\nBooking Status   : " << getStatus();

        out << "\nTicket Price     : Rs. "
            << fixed << setprecision(2)
            << passenger->ticketPrice;

        out << "\nPayment Status   : ";

        if (passenger->paymentStatus == PAYMENT_PAID)
            out << "PAID";
        else
            out << "PAYMENT PENDING";

        if (passenger->status == STATUS_WAITING)
            out << "\nDocument Type    : WAITING LIST RECEIPT";
        else if (passenger->status == STATUS_RAC)
            out << "\nDocument Type    : RAC RESERVATION RECEIPT";
        else
            out << "\nDocument Type    : CONFIRMED RESERVATION";

        out << "\nGenerated On     : " << dateTime;

        out << "\n========================================\n";
        out << "       Thank you for booking!          \n";
        out << "========================================\n";
    }

    bool saveToFile() const
    {
        string filename =
            "Ticket_" + to_string(passenger->pnr) + ".txt";

        ofstream file(filename);

        if (!file.is_open())
            return false;

        display(file);

        file.close();

        if (!file)
            return false;

        cout << "\nTicket saved successfully as: "
             << filename << endl;

        return true;
    }
};

/* C-compatible ticket generation function */

extern "C"
{

int generateTicket(const struct Passenger *p)
{
    if (p == nullptr)
    {
        cout << "\nInvalid passenger record.\n";
        return 0;
    }

    if (p->pnr <= 0)
    {
        cout << "\nTicket cannot be generated without a PNR.\n";
        return 0;
    }

    if (p->status == STATUS_CANCELLED)
    {
        cout << "\nThis booking has been cancelled.\n";
        return 0;
    }

    if (p->coachType < 0 || p->coachType >= MAX_COACHES)
    {
        cout << "\nInvalid coach information.\n";
        return 0;
    }

    Ticket ticket(p);

    cout << "\nGenerating ticket...\n";

    ticket.display(cout);

    if (!ticket.saveToFile())
    {
        cout << "\nUnable to save ticket file.\n";
        return 0;
    }

    return 1;
}

}