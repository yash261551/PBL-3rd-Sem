#ifndef SORTING_H
#define SORTING_H

#include "passenger.h"

void sortByID(struct Passenger arr[], int n);
void sortByAge(struct Passenger arr[], int n);
void sortByPriority(struct Passenger arr[], int n);

void displaySortedPassengers(struct Passenger arr[], int n);

#endif