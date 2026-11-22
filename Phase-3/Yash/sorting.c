#include <stdio.h>

#include "sorting.h"

void sortByID(struct Passenger arr[], int n)
{
    int i, j;
    struct Passenger temp;

    for(i = 0; i < n - 1; i++)
    {
        for(j = 0; j < n - i - 1; j++)
        {
            if(arr[j].id > arr[j + 1].id)
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void sortByAge(struct Passenger arr[], int n)
{
    int i, j;
    struct Passenger temp;

    for(i = 0; i < n - 1; i++)
    {
        for(j = 0; j < n - i - 1; j++)
        {
            if(arr[j].age > arr[j + 1].age)
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void sortByPriority(struct Passenger arr[], int n)
{
    int i, j;
    struct Passenger temp;

    for(i = 0; i < n - 1; i++)
    {
        for(j = 0; j < n - i - 1; j++)
        {
            if(arr[j].priority > arr[j + 1].priority)
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void displaySortedPassengers(struct Passenger arr[], int n)
{
    int i;

    printf("\n-----------------------------------\n");

    for(i = 0; i < n; i++)
    {
        printf("ID: %d | Name: %s | Age: %d | Priority: %d | Seat: %d\n",
               arr[i].id,
               arr[i].name,
               arr[i].age,
               arr[i].priority,
               arr[i].seatNo);
    }

    printf("-----------------------------------\n");
}