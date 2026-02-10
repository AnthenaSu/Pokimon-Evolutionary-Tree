// COMP2521 - Assignment 1
// Concrete representation of the evolutionary tree ADT

#ifndef EVO_TREE_STRUCTS_H
#define EVO_TREE_STRUCTS_H

// DO NOT CHANGE THE NAME OF THIS STRUCT
struct evoTree {
    struct node *root;  // DO NOT MODIFY/REMOVE THIS FIELD

    // You may add more fields here if needed
};

// DO NOT CHANGE THE NAME OF THIS STRUCT
struct node {
    int speciesNumber;  // DO NOT MODIFY/REMOVE THIS FIELD
    char *speciesName;  // DO NOT MODIFY/REMOVE THIS FIELD
    struct node *left;  // DO NOT MODIFY/REMOVE THIS FIELD
    struct node *right; // DO NOT MODIFY/REMOVE THIS FIELD

    // You may add more fields here if needed
};

// You may define more structs here if needed

typedef void *Item;
typedef struct stack *Stack;
typedef struct queue *Queue;

struct stack {
	struct stackNode *head;
	int size;
};

struct stackNode {
	Item item;
	struct stackNode *next;
};

struct queue {
	struct queueNode *head;
	struct queueNode *tail;
	int size;
};

struct queueNode {
	Item item;
	struct queueNode *next;
};


#endif

