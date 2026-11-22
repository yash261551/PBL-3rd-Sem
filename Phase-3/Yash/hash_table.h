#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "passenger.h"

#define HASH_SIZE 10

struct HashNode
{
    struct Passenger passenger;
    struct HashNode *next;
};

int hashFunction(int id);

void insertHash(struct Passenger p);

struct HashNode* searchHash(int id);

void displayHash();

#endif