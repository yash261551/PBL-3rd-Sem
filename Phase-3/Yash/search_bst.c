#include <stdio.h>
#include <stdlib.h>

#include "search_bst.h"

struct BSTNode* insertBST(struct BSTNode *root, struct Passenger p)
{
    if(root == NULL)
    {
        struct BSTNode *newNode;

        newNode = (struct BSTNode*)malloc(sizeof(struct BSTNode));

        if(newNode == NULL)
        {
            printf("Memory allocation failed.\n");
            return NULL;
        }

        newNode->passenger = p;
        newNode->left = NULL;
        newNode->right = NULL;

        return newNode;
    }

    if(p.id < root->passenger.id)
    {
        root->left = insertBST(root->left, p);
    }
    else if(p.id > root->passenger.id)
    {
        root->right = insertBST(root->right, p);
    }
    else
    {
        printf("Passenger ID already exists.\n");
    }

    return root;
}

struct BSTNode* searchBST(struct BSTNode *root, int id)
{
    if(root == NULL)
        return NULL;

    if(root->passenger.id == id)
        return root;

    if(id < root->passenger.id)
        return searchBST(root->left, id);

    return searchBST(root->right, id);
}

void inorderBST(struct BSTNode *root)
{
    if(root == NULL)
        return;

    inorderBST(root->left);

    printf("\nID: %d | Name: %s | Age: %d | Seat: %d",
           root->passenger.id,
           root->passenger.name,
           root->passenger.age,
           root->passenger.seatNo);

    inorderBST(root->right);
}