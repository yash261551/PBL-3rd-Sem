#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"

struct HashNode *hashTable[HASH_SIZE] = {NULL};

int hashFunction(int id)
{
    return id % HASH_SIZE;
}

void insertHash(struct Passenger p)
{
    int index;
    struct HashNode *newNode;

    index = hashFunction(p.id);

    newNode = (struct HashNode*)malloc(sizeof(struct HashNode));

    if(newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    newNode->passenger = p;

    newNode->next = hashTable[index];

    hashTable[index] = newNode;
}

struct HashNode* searchHash(int id)
{
    int index;
    struct HashNode *current;

    index = hashFunction(id);

    current = hashTable[index];

    while(current != NULL)
    {
        if(current->passenger.id == id)
            return current;

        current = current->next;
    }

    return NULL;
}

void displayHash()
{
    int i;
    struct HashNode *current;

    printf("\n========== HASH TABLE ==========\n");

    for(i = 0; i < HASH_SIZE; i++)
    {
        printf("\nIndex %d: ", i);

        current = hashTable[i];

        while(current != NULL)
        {
            printf("ID %d -> ", current->passenger.id);

            current = current->next;
        }

        printf("NULL");
    }

    printf("\n");
}