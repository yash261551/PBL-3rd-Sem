#ifndef COACH_H
#define COACH_H

#include <string>
using namespace std;

class Coach
{
private:
    string coachName;
    int totalSeats;
    double ticketPrice;

public:
    Coach(string name, int seats, double price);

    string getCoachName();
    int getTotalSeats();
    double getTicketPrice();

    void displayCoach();
};

#endif