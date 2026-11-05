#include <iostream>
#include "Coach.h"
using namespace std;

Coach::Coach(string name, int seats, double price)
{
    coachName = name;
    totalSeats = seats;
    ticketPrice = price;
}

string Coach::getCoachName()
{
    return coachName;
}

int Coach::getTotalSeats()
{
    return totalSeats;
}

double Coach::getTicketPrice()
{
    return ticketPrice;
}

void Coach::displayCoach()
{
    cout << "\nCoach: " << coachName;
    cout << "\nTotal Seats: " << totalSeats;
    cout << "\nTicket Price: Rs. " << ticketPrice << endl;
}