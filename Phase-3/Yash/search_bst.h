#ifndef SEARCH_BST_H
#define SEARCH_BST_H

#include "passenger.h"

struct BSTNode
{
    struct Passenger passenger;
    struct BSTNode *left;
    struct BSTNode *right;
};

struct BSTNode* insertBST(struct BSTNode *root, struct Passenger p);
struct BSTNode* searchBST(struct BSTNode *root, int id);
void inorderBST(struct BSTNode *root);

#endif