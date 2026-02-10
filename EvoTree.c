// COMP2521 - Assignment 1
// Name: Anthena Su 
// zID: z5640267
// date: 08.07.2026
// Implementation of the Evolutionary Tree ADT
// Description: A Pokémon BST evolutionary tree project that 
// manages species data, supports ancestor/path queries, and inserts intermediate. 

// Acknowledgements:
// - Implementation of the Queue ADT using a linked list
// https://cgi.cse.unsw.edu.au/~cs2521/25T2/lab/4/questions
// - Implementation of the Stack ADT using a linked list
// https://cgi.cse.unsw.edu.au/~cs2521/25T2/lab/4/questions

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EvoTree.h"
#include "EvoTreeStructs.h"

////////////////////////////////////////////////////////////////////////
// My Prototypes
////////////////////////////////////////////////////////////////////////
static void freeTreeNodes(struct node *root);
static int CountTotalNodes(struct node *root);
static void NodeInOrder(struct node *root, FILE *out, int *first);
static char *NodeFind(struct node *root, int speciesNumber);
static struct stackNode *newNode(Item it);
static void bstInOrder(struct node *root, int speciesNumber, int min, int max, 
                        Species relatives[], int *index);
static Species NodeCommonAncestor(struct node *curr, int min, int max); 
static int findMin(EvoTree et, int speciesNumbers[], int size);
static int findMax(EvoTree et, int speciesNumbers[], int size);
static struct node *findAncestor(struct node *root, int ancestorNumber);
int ADTevolutionPath(struct node *ancestorNode, int speciesNumberA, 
                    int speciesNumberB, Species path[]);
int ADTevolutionQueue(struct node *ancestorNode, int speciesNumberA, 
                    int speciesNumberB, Species path[], int *totalSize, 
                    int sizeOfStack);
static void bstPreOrder(EvoTree et, struct node *root, int gap, 
                        int *totalGenerated);
static int newSpeciesAmount(int max, int min, int acceptableGap);
static char *nameGenerator(struct node *max, struct node *min);
static char *multiNameGenerator(char *maxName, char *minName, int *index);

// Stack
Stack StackNew(void);
void StackFree(Stack s);
void StackPush(Stack s, Item item);
Item StackPop(Stack s);
Item StackTop(Stack s);
int StackSize(Stack s);
bool StackIsEmpty(Stack s);

// Queue
static Queue QueueNew(void);
static void QueueFree(Queue q);
static void QueueEnqueue(Queue q, Item it);
static struct queueNode *newQueueNode(Item item);
static Item QueueDequeue(Queue q);
// static Item QueueFront(Queue q);
static int QueueSize(Queue q);
// static bool QueueIsEmpty(Queue q);

///////////////////////////////////////////////////////////////////////////////
// Part 1: Basic Operations

/**
 * Creates a new empty evolutionary tree
 */
EvoTree EvoTreeNew(void) {
    struct evoTree *newTree = malloc(sizeof(struct evoTree));
    newTree->root = NULL;
    return newTree;
}

/**
 * Frees all memory allocated to the evolutionary tree
 */
void EvoTreeFree(EvoTree et) {
    if (et == NULL) return;
    freeTreeNodes(et->root);
    free(et);
}

/**
 * helper function
 * Frees all node memory allocated to the evolutionary tree
 */
static void freeTreeNodes(struct node *root) {
    if (root == NULL) return;
    freeTreeNodes(root->left);
    freeTreeNodes(root->right);
    free(root->speciesName);
    free(root);
}

/**
 * Inserts a new species into the evolutionary tree. Does nothing if the
 * supplied species number already exists in the tree.
 */
void EvoTreeInsert(EvoTree et, int speciesNumber, char *speciesName) {

    if (et == NULL) return;

    struct node *newnode = malloc(sizeof(struct node));
    newnode->speciesNumber = speciesNumber;
    newnode->speciesName = strdup(speciesName);
    newnode->left = NULL;
    newnode->right = NULL;

    if (et->root == NULL) {
        et->root = newnode;
        return; 
    }
    
    struct node *curr = et->root;

    while(1) {
        // case 1: exact same -> do nothing
        if (speciesNumber == curr->speciesNumber) {
            free(newnode->speciesName);
            free(newnode);
            return; 
        }
        if (speciesNumber < curr->speciesNumber) {
            if (curr->left == NULL) {
                curr->left = newnode;
                return;
            }
            curr = curr->left;
        }
        else {
            if (curr->right == NULL) {
                curr->right = newnode;
                return;
            }
            curr = curr->right;
        }
    }
    
}

/**
 * Returns the number of species in the evolutionary tree
 */
int EvoTreeNumSpecies(EvoTree et) {

    // tree is empty
    if (et == NULL || et->root == NULL) {
        return 0;
    }

    return CountTotalNodes(et->root);
}

// helper function for EvoTreeNumSpecies
// using recursion to count 
// total nodes = 1 (root) + left-side nodes + right-side nodes
static int CountTotalNodes(struct node *root) { 
    struct node *curr = root; 

    // Base case
    if (curr == NULL) {
        return 0;
    }

    return 1 + CountTotalNodes(curr->left) +  CountTotalNodes(curr->right);
}

/**
 * Prints all of the species in the evolutionary tree to the given file
 * in ascending order of species number. The list of species should be
 * printed inside a pair of curly braces, with species separated by a
 * comma and space, and each species should be printed inside a pair of
 * parentheses with its species number and name separated by a comma and
 * space.
 */
void EvoTreePrint(EvoTree et, FILE *out) {
    if (et == NULL || et->root == NULL) {
        fprintf(out, "{}");
        return;
    }

    int first = 1;
    fprintf(out, "{");
    NodeInOrder(et->root, out, &first);
    fprintf(out, "}");
}

// helper function for EvoTreePrint
// prints with format 
static void NodeInOrder(struct node *root, FILE *out, int *first) {
    if (root == NULL) return;

    NodeInOrder(root->left, out, first);

    if (*first == 1) {
        fprintf(out, "(%d, %s)", root->speciesNumber, root->speciesName);
        *first = 0;
    } else {
        fprintf(out, ", (%d, %s)", root->speciesNumber, root->speciesName);
    }

    NodeInOrder(root->right, out, first);
}


////////////////////////////////////////////////////////////////////////
// Part 2: Classifying Species

/**
 * Returns the name of the species with the given species number, or the
 * string "undefined" if there is no species with that species number.
 */
char *EvoTreeFind(EvoTree et, int speciesNumber) {
    
    if (et == NULL || et->root == NULL) {
        return "undefined";
    }

    return NodeFind(et->root, speciesNumber);
}

// helper function for EvoTreeFind
// return species name if found
// else compare the number and traverses down to the tree.
static char *NodeFind(struct node *root, int speciesNumber) {
    
    struct node *curr = root; 
    while (curr != NULL) {
        // find !
        if (speciesNumber == curr->speciesNumber) {
            return curr->speciesName;
        }
        if (speciesNumber < curr->speciesNumber) {
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    } 

    // reach bottom but not found
    return "undefined";
}

/**
 * Given a species number, this function stores the species that make up
 * its evolutionary history in the given `history` array in order of
 * recency, and returns the number of species stored.
 */
// array with type struct species 
int EvoTreeHistory(EvoTree et, int speciesNumber, Species ancestors[]) {
    
    // create a new stack 
    Stack s = StackNew();

    // push (store) everything 
    struct node *curr = et->root;
    while(1) {
        StackPush(s, curr);
        if (speciesNumber == curr->speciesNumber) {
            break; 
        }

        if (speciesNumber < curr->speciesNumber) {
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    }

    int total = StackSize(s);

    // pop them & store in array
    for (int i = 0; i < total; i++) {
        struct node *x = StackPop(s);
        (ancestors)[i].number = x->speciesNumber;
        (ancestors)[i].name = x->speciesName;
    }

    StackFree(s);
    return total;
}



/**
 * Given a species number and a positive integer `width`, this function
 * stores all relatives of the species in the given `relatives` array in
 * ascending order of species number, and returns the number of species
 * stored.
 * Two different species are called relatives if their species numbers
 * differ by at most `width`.
 */

// LNR
int EvoTreeRelatives(EvoTree et, int speciesNumber,
                     int width, Species relatives[]) {
    
    int min = speciesNumber - width;
    int max = speciesNumber + width;

    int index = 0;
    bstInOrder(et->root, speciesNumber, min, max, relatives, &index);
    return index;
}

// helper function for EvoTreeRelatives
// uses in-order traversal (LNR)
static void bstInOrder(
    struct node *root, 
    int speciesNumber, 
    int min, 
    int max, 
    Species relatives[], 
    int *index) {

    if (root == NULL) return;

    struct node *curr = root;
    bstInOrder(curr->left, speciesNumber, min, max, relatives, index);

    // passing conditions:
    // 1. in the range [min, max]
    // 2. cannot be self
    if (min <= curr->speciesNumber && curr->speciesNumber <= max && 
        curr->speciesNumber != speciesNumber) {
        relatives[(*index)].name = curr->speciesName;
        relatives[(*index)].number = curr->speciesNumber;
        (*index) ++;
    }

    bstInOrder(curr->right, speciesNumber, min, max, relatives, index);
}

////////////////////////////////////////////////////////////////////////
// Common Ancestry

/**
 * Given an array of species numbers and its size, this function returns
 * the most recent common ancestor of all the valid species numbers in
 * the array.
 */
Species EvoTreeCommonAncestor(EvoTree et, int speciesNumbers[], int size) {

    // find min and max
    int min = findMin(et, speciesNumbers, size);
    int max = findMax(et, speciesNumbers, size);

    return NodeCommonAncestor(et->root, min, max);
}

// helper function for EvoTreeCommonAncestor
// finds the lowest common ancestor of a group of species in a tree
// by identifying the node whose speciesNumber lies between 
// the minimum and maximum values in the given list.
static Species NodeCommonAncestor(struct node *curr, int min, int max) {
    if (curr == NULL) {
        return (Species){-1, "undefined"};
    }

    if (curr->speciesNumber > max) {
        // All species are in the left subtree
        return NodeCommonAncestor(curr->left, min, max);
    } else if (curr->speciesNumber < min) {
        // All species are in the right subtree
        return NodeCommonAncestor(curr->right, min, max);
    } else {
        // This node is between min and max — common ancestor
        return (Species){curr->speciesNumber, curr->speciesName};
    }
}

// helper function for EvoTreeCommonAncestor
// finds the minimum valid species number in the arrray
static int findMin(EvoTree et, int speciesNumbers[], int size) {
    int min = speciesNumbers[size - 1];
    for (int i = size - 1; i >= 0; i--) {
        if (strcmp(EvoTreeFind(et, speciesNumbers[i]), "undefined") != 0) {
            min = speciesNumbers[i];
        }
    }

    return min;
}

// helper function for EvoTreeCommonAncestor 
// finds the maximum valid species number in the arrray
static int findMax(EvoTree et, int speciesNumbers[], int size) {
    int max = speciesNumbers[0];
    for (int i = 0; i < size; i++) {
        if (strcmp(EvoTreeFind(et, speciesNumbers[i]), "undefined") != 0) {
            max = speciesNumbers[i];
        }
    }

    return max;
}


/**
 * Given two valid species numbers, this function stores the path that
 * connects the two species (from `speciesNumberA` to `speciesNumberB`)
 * in the provided `path` array, and returns the number of species
 * stored.
 */

// The strategy is to first find common ancestor 
// then use stact -> common ancestor -> queue 
int EvoTreeEvolutionPath(EvoTree et, int speciesNumberA,
                         int speciesNumberB, Species path[]) {
    
    int speciesNumbers[] = {speciesNumberA, speciesNumberB};
    Species ancestor = EvoTreeCommonAncestor(et, speciesNumbers, 2);
    printf("%d\n", ancestor.number);
    int ancestorNumber = ancestor.number;

    struct node *ancestorNode = findAncestor(et->root, ancestorNumber);
    printf("%d\n", ancestorNode->speciesNumber);
    return ADTevolutionPath(ancestorNode, speciesNumberA, speciesNumberB, path);
}

// Part I: Stack for EvoTreeEvolutionPath
// This function builds the evolution path from speciesNumberA 
// to the common ancestor using a stack, 
// then continues to speciesNumberB via queue in ADTevolutionQueue.
int ADTevolutionPath(struct node *ancestorNode, int speciesNumberA, 
                    int speciesNumberB, Species path[]) {
    int totalSize = 0;

    // create a new stack 
    Stack s = StackNew();
    struct node *curr = ancestorNode;
    while(1) {
        printf("species number: %d\n", curr->speciesNumber);
        StackPush(s, curr);
        // reach speciesNumberA -> leave the while loop
        if (curr->speciesNumber == speciesNumberA) {
            break;
        }
        if (curr->speciesNumber > speciesNumberA) {
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    }

    int sizeOfStack = StackSize(s);
    totalSize += StackSize(s);

    // store into array and pop respectivetly
    for (int i = 0; i < totalSize; i++) {
        struct node *pop = StackPop(s);
        path[i].number = pop->speciesNumber;
        path[i].name = pop->speciesName;
    }
    StackFree(s);

    // go to queue (part II)
    ADTevolutionQueue(ancestorNode, speciesNumberA, 
                    speciesNumberB, path, &totalSize, sizeOfStack);
    
    return totalSize;
}

// Part II: Queue for EvoTreeEvolutionPath
// Builds the evolution path from the common ancestor to speciesNumberB 
// using a queue, avoiding duplicate ancestor entry.
int ADTevolutionQueue(struct node *ancestorNode, int speciesNumberA, 
                    int speciesNumberB, Species path[], int *totalSize, 
                    int sizeOfStack) {
    
    // create new queue
    Queue q = QueueNew();
    struct node *current = ancestorNode;
    while(1) {
        QueueEnqueue(q, current);
        if (current->speciesNumber == speciesNumberB) {
            break;
        }
        if (current->speciesNumber < speciesNumberB) {
            current = current->right;
        }
        else {
            current = current->left;
        }
    }
    (*totalSize) += QueueSize(q);
    // ancestor appears twice 
    (*totalSize) --;   

    // store into array and pop respectivetly
    for (int i = sizeOfStack; i < *totalSize; i++) {
        struct node *pop = QueueDequeue(q);
        // don't want to store ancestor again;
        if (pop->speciesNumber == ancestorNode->speciesNumber) {
            i--;
            continue;
        }
        path[i].number = pop->speciesNumber;
        path[i].name = pop->speciesName;
    }
    QueueFree(q);
    
    // totalSize = the path (return value)
    return *totalSize;
}

// helper function for EvoTreeEvolutionPath
// Given an ancestorNumber, this function returns a pointer to the node 
// containing that species number in the binary search tree. 
// Returns NULL if the species is not found.
static struct node *findAncestor(struct node *root, int ancestorNumber) {
    
    struct node *curr = root; 
    while (curr != NULL) {
        // find !
        if (ancestorNumber == curr->speciesNumber) {
            return curr;
        }
        if (ancestorNumber < curr->speciesNumber) {
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    } 

    return curr;

}

////////////////////////////////////////////////////////////////////////
// Tree Reconstruction

/**
 * Constructs and returns an evolutionary tree from its post-order
 * traversal provided in the `data` array.
 */
EvoTree EvoTreeConstruct(Species data[], int size) {
  
    // create new tree & add root (last element in data list)
    EvoTree tree = EvoTreeNew();
    EvoTreeInsert(tree, data[size - 1].number, data[size - 1].name);
    int rightIndex = 0;

    // find root->right
    for (int i = size - 2; i >= 0; i--) {
        if (data[i].number > data[size - 1].number) {
            struct node *newNode = malloc(sizeof(struct node));
            newNode->speciesNumber = data[i].number;
            newNode->speciesName = strdup(data[i].name);
            newNode->left = NULL;
            newNode->right = NULL;
            tree->root->right = newNode;
            rightIndex = i;
            break;
        }
    }

    // find root->left
    for (int i = rightIndex - 1; i >= 0; i--) {
        if (data[i].number < data[size - 1].number) {
            struct node *newNode = malloc(sizeof(struct node));
            newNode->speciesNumber = data[i].number;
            newNode->speciesName = strdup(data[i].name);
            newNode->left = NULL;
            newNode->right = NULL;
            tree->root->left = newNode;
            break;
        }
    }

    int index = size - 2; 

    // add rest of them
    while (index >= 0) {
       EvoTreeInsert(tree, data[index].number, data[index].name);
       index --;
    }
    return tree;
}


////////////////////////////////////////////////////////////////////////
// Generating Intermediate Species
/*
 * Generates intermediate species for all unacceptable gaps in evolutionary
 * tree. A gap is unacceptable if the difference in species numbers between a
 * parent and child exceeds the given acceptableGap.
 * 
 * This function modifies the tree in-place by inserting new species nodes
 * and returns the total number of new species generated.
*/
int EvoTreeGenerateIntermediate(EvoTree et, int acceptableGap) {
    
    int totalGenerated = 0;

    // pre-order traversal
    bstPreOrder(et, et->root, acceptableGap, &totalGenerated);
    
    return totalGenerated;
}

/**
 * A helper function that performs a pre-order traversal of the tree 
 * and inserts intermediate species 
 * wherever the gap between a node and its child exceeds the acceptable gap.
 * 
 * For each large gap, it may insert one or multiple intermediate species.
*/
static void bstPreOrder(EvoTree et, struct node *root, 
                        int gap, int *totalGenerated) {
    if (et == NULL || root == NULL) return;

    int index = 1;

    // Generate intermediates for left child
    if (root->left != NULL && newSpeciesAmount(root->speciesNumber, 
                    root->left->speciesNumber, gap) != 0) {
        int newAmount = newSpeciesAmount(root->speciesNumber, 
        root->left->speciesNumber, gap);
        (*totalGenerated) += newAmount;

        // only one intermediates 
        if (newAmount == 1) {
            char *newName = nameGenerator(root, root->left);
            EvoTreeInsert(et, root->speciesNumber - gap, newName);
            free(newName);
        }
        else if (newAmount > 1) {
            // multiple new intermediates 
            int max = root->speciesNumber - gap;
            int min = root->left->speciesNumber;
            char *maxName = root->speciesName;
            char *minName = root->left->speciesName;
            for (int i = max; i > min; i -= gap) {
                char *newName = multiNameGenerator(maxName, minName, &index);

                struct node *newleft = malloc(sizeof(struct node));
                newleft->speciesNumber = i;
                newleft->speciesName = strdup(newName);
                newleft->left = root->left; 
                newleft->right = NULL;
                root->left = newleft;

                // printf("MULTIPLE name: %s, number: %d\n", newName, i);
                free(newName); 
                root = root->left;
                index += 1;
            }
            index = 1;
        }
    }

    // Generate intermediates for right child
    if (root->right != NULL && newSpeciesAmount(root->right->speciesNumber, 
        root->speciesNumber, gap) != 0) {
        int newAmount = newSpeciesAmount(root->right->speciesNumber, 
                        root->speciesNumber, gap);
        (*totalGenerated) += newAmount;
        
        if (newAmount == 1) {
            char *newName = nameGenerator(root, root->right);
            EvoTreeInsert(et, root->right->speciesNumber - gap, newName); 
            free(newName); 
        }
        else if (newAmount > 1) {
            index = 1;
            int max = root->right->speciesNumber - gap;
            int min = root->speciesNumber; 
            char *maxName = root->speciesName;
            char *minName = root->right->speciesName;
            for (int i = max; i > min; i -= gap) {
                char *newName = multiNameGenerator(maxName, minName, &index);
                
                struct node *newright = malloc(sizeof(struct node));
                newright->speciesNumber = i;
                newright->speciesName = strdup(newName);
                newright->left = NULL;
                newright->right = root->right;
                root->right = newright;
                
                // printf("MULTIPLE name: %s, number: %d\n", newName, i);
                free(newName); 
                root = root->right;
                index += 1;
            }
            index = 1;
        }
    }
    // Recursively process left and right subtrees
    bstPreOrder(et, root->left, gap, totalGenerated);
    bstPreOrder(et, root->right, gap, totalGenerated);
}

// A helper function for EvoTreeGenerateIntermediate()
// Given a range (max - min) and an acceptableGap, this function calculates
// the amount of new species needed to be generated.
// If range <= acceptableGap, current gap is fine. Do nothing and return 0.
// On the other hand, count the number using the following formulas.  
static int newSpeciesAmount(int max, int min, int acceptableGap) {
    int range = max - min;
    if (range <= acceptableGap) {
        return 0;
    };

    // need intermediate species
    if (range % acceptableGap == 0) {
        return (range / acceptableGap) - 1;
    }
    return range / acceptableGap;
}

// helper function for EvoTreeGenerateIntermediate()
// Generates a name for a single intermediate species 
// by concatenating the names of the parent (max) and child (min) species.
static char *nameGenerator(struct node *max, struct node *min) {
    char *maxName = max->speciesName;
    char *minName = min->speciesName;

    int maxLength = strlen(maxName);
    int minLength = strlen(minName);
    int newLength = maxLength + minLength;

    char *newname = malloc(sizeof(char) * (newLength + 1));

    strcpy(newname, maxName);
    strcpy(newname + maxLength, minName);
    newname[newLength] = '\0';

    return newname;
}

// helper function for EvoTreeGenerateIntermediate()
// Generates a unique name for one of multiple intermediate species 
// by concatenating max and min species names, followed by a number suffix.
static char *multiNameGenerator(char *maxName, char *minName, int *index) {

    int maxLength = strlen(maxName);
    int minLength = strlen(minName);
    int newLength = maxLength + minLength;

    char *newname = malloc(sizeof(char) * (newLength + 2));

    strcpy(newname, maxName);
    strcpy(newname + maxLength, minName);
    int charIndex = *index + 48;
    newname[newLength] = (char) charIndex;
    newname[newLength + 1] = '\0';

    return newname;
}

////////////////////////////////////////////////////////////////////////
// Stack 
// Implementation of the Stack ADT using a linked list
// This file was taken from 
// https://cgi.cse.unsw.edu.au/~cs2521/25T2/lab/4/questions

/**
 * Creates a new empty stack
 * Time complexity: O(1)
 */
Stack StackNew(void) {
	Stack s = malloc(sizeof(*s));

	s->head = NULL;
	s->size = 0;
	return s;
}

/**
 * Frees all memory allocated to the stack
 * Time complexity: O(n)
 */
void StackFree(Stack s) {
	struct stackNode *curr = s->head;
	while (curr != NULL) {
		struct stackNode *temp = curr;
		curr = curr->next;
		free(temp);
	}
	free(s);
}

/**
 * Adds an item to the top of the stack
 * Time complexity: O(1)
 */
void StackPush(Stack s, Item item) {
	struct stackNode *n = newNode(item);
	n->next = s->head;
	s->head = n;
	s->size++;
}

/**
 * Creates a new node 
 * Time complexity: O(1)
 */
static struct stackNode *newNode(Item item) {
	struct stackNode *n = malloc(sizeof(*n));
	if (n == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}

	n->item = item;
	n->next = NULL;
	return n;
}

/**
 * Removes the item at the top of the stack and returns it
 * Assumes that the stack is not empty
 * Time complexity: O(1)
 */
Item StackPop(Stack s) {
	if (s->size == 0) {
		fprintf(stderr, "error: stack is empty\n");
		exit(EXIT_FAILURE);
	}

	struct stackNode *newHead = s->head->next;
	Item item = s->head->item;

	free(s->head);
	s->head = newHead;
	s->size--;

	return item;
}

/**
 * Returns the item at the top of the stack without removing it
 * Assumes that the stack is not empty
 * Time complexity: O(1)
 */
Item StackTop(Stack s) {
	if (s->size == 0) {
		fprintf(stderr, "error: stack is empty\n");
		exit(EXIT_FAILURE);
	}

	return s->head->item;
}

/**
 * Returns the number of items in the stack
 * Time complexity: O(1)
 */
int StackSize(Stack s) {
	return s->size;
}

/**
 * Returns true if the stack is empty, and false otherwise
 * Time complexity: O(1)
 */
bool StackIsEmpty(Stack s) {
	return s->size == 0;
}


////////////////////////////////////////////////////////////////////////
// Queue 
// Implementation of the Queue ADT using a linked list
// This file was taken from 
// https://cgi.cse.unsw.edu.au/~cs2521/25T2/lab/4/questions

/**
 * Creates a new empty queue
 * Time complexity: O(1)
 */
static Queue QueueNew(void) {
	Queue q = malloc(sizeof(*q));
	if (q == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}
	
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
	return q;
}

/**
 * Frees all resources associated with the given queue
 * Time complexity: O(n)
 */
static void QueueFree(Queue q) {
	struct queueNode *curr = q->head;
	while (curr != NULL) {
		struct queueNode *temp = curr;
		curr = curr->next;
		free(temp);
	}
	free(q);
}

/**
 * Adds an item to the end of the queue
 * Time complexity: O(1)
 */
static void QueueEnqueue(Queue q, Item item) {
	struct queueNode *n = newQueueNode(item);
	if (q->size == 0) {
		q->head = n;
	} else {
		q->tail->next = n;
	}
	q->tail = n;
	q->size++;
}

/**
 * Creates a new queue node
 * Time complexity: O(1)
 */
static struct queueNode *newQueueNode(Item item) {
	struct queueNode *n = malloc(sizeof(*n));
	if (n == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}
	
	n->item = item;
	n->next = NULL;
	return n;
}

/**
 * Removes an item from the front of the queue and returns it
 * Assumes that the queue is not empty
 * Time complexity: O(1)
 */
static Item QueueDequeue(Queue q) {
	if (q->size == 0) {
		fprintf(stderr, "error: queue is empty\n");
		exit(EXIT_FAILURE);
	}
	
	struct queueNode *newHead = q->head->next;
	Item item = q->head->item;

	free(q->head);
	q->head = newHead;
	if (newHead == NULL) {
		q->tail = NULL;
	}
	q->size--;

	return item;
}

/**
 * Gets the size of the given queue
 * Time complexity: O(1)
 */
static int QueueSize(Queue q) {
	return q->size;
}
