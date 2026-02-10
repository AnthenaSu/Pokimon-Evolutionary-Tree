// COMP2521 - Assignment 1
// Interface to the Evolutionary Tree ADT

// !!! DO NOT MODIFY THIS FILE !!!

#ifndef EVO_TREE_H
#define EVO_TREE_H

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct species {
    int number;
    char *name;
} Species;

typedef struct evoTree *EvoTree;

////////////////////////////////////////////////////////////////////////
// Basic Operations

/**
 * Creates a new empty evolutionary tree
 */
EvoTree EvoTreeNew(void);

/**
 * Frees all memory allocated to the evolutionary tree
 */
void EvoTreeFree(EvoTree et);

/**
 * Inserts a new species into the evolutionary tree. Does nothing if the
 * supplied species number already exists in the tree.
 */
void EvoTreeInsert(EvoTree et, int number, char *name);

/**
 * Returns the number of species in the evolutionary tree
 */
int EvoTreeNumSpecies(EvoTree et);

/**
 * Prints all of the species in the evolutionary tree to the given file
 * in ascending order of species number. The list of species should be
 * printed inside a pair of curly braces, with species separated by a
 * comma and space, and each species should be printed inside a pair of
 * parentheses with its species number and name separated by a comma and
 * space.
 */
void EvoTreePrint(EvoTree et, FILE *out);

////////////////////////////////////////////////////////////////////////
// Classifying Species

/**
 * Returns the name of the species with the given species number, or the
 * string "undefined" if there is no species with that species number.
 */
char *EvoTreeFind(EvoTree et, int speciesNumber);

/**
 * Given a species number, this function stores the species that make up
 * its evolutionary history in the given `history` array in order of
 * recency, and returns the number of species stored.
 */
int EvoTreeHistory(EvoTree et, int speciesNumber, Species history[]);

/**
 * Given a species number and a positive integer `width`, this function
 * stores all relatives of the species in the given `relatives` array in
 * ascending order of species number, and returns the number of species
 * stored.
 * Two different species are called relatives if their species numbers
 * differ by at most `width`.
 */
int EvoTreeRelatives(EvoTree et, int speciesNumber, int width,
                     Species relatives[]);

////////////////////////////////////////////////////////////////////////
// Common Ancestry

/**
 * Given an array of species numbers and its size, this function returns
 * the most recent common ancestor of all the valid species numbers in
 * the array.
 */
Species EvoTreeCommonAncestor(EvoTree et, int speciesNumbers[], int size);

/**
 * Given two valid species numbers, this function stores the path that
 * connects the two species (from `speciesNumberA` to `speciesNumberB`)
 * in the provided `path` array, and returns the number of species
 * stored.
 */
int EvoTreeEvolutionPath(EvoTree et, int speciesNumberA,
                         int speciesNumberB, Species path[]);

////////////////////////////////////////////////////////////////////////
// Tree Reconstruction

/**
 * Constructs and returns an evolutionary tree from its post-order
 * traversal provided in the `data` array.
 */
EvoTree EvoTreeConstruct(Species data[], int size);

////////////////////////////////////////////////////////////////////////
// Generating Intermediate Species

/**
 * Given an acceptable gap between species, this function generates
 * intermediate species to fill all unacceptable gaps within the given
 * evolutionary tree, and returns the number of species generated.
 */
int EvoTreeGenerateIntermediate(EvoTree et, int acceptableGap);

////////////////////////////////////////////////////////////////////////

#endif

