# Pokimon-Evolutionary-Tree
Implemented an Evolutionary Tree Abstract Data Types (ADT) in C using a binary search tree to model Pokémon species relationships. Supports insertion, traversal, ancestry and relative queries, evolution path finding, tree reconstruction from traversal data, and generation of intermediate species using stacks, queues, and recursive algorithms.

## Project Structure

| File | Description |
|-----|-------------|
| `Makefile` | Controls compilation and build targets for the project. |
| `EvoTree.h` | Public interface for the Evolutionary Tree ADT (provided; not modified). |
| `EvoTree.c` | Implementation of the Evolutionary Tree ADT. |
| `EvoTreeStructs.h` | Definitions of internal data structures used by the ADT. |
| `testEvoTree.c` | Driver program containing basic tests for the Evolutionary Tree ADT. |
| `analysis.txt` | Time complexity analysis for selected ADT functions. |

## Features
- **BST-based species storage** (`speciesNumber` as the key)
- **Basic operations:** create, insert, find, count, print (in-order)
- **Relationship queries:**
  - `EvoTreeHistory()` — path from root to a species (most recent first)
  - `EvoTreeRelatives()` — species within a numeric `width` range
  - `EvoTreeCommonAncestor()` — most recent shared ancestor of multiple species
  - `EvoTreeEvolutionPath()` — path between two species via common ancestor
- **Tree reconstruction:** `EvoTreeConstruct()` builds from post-order traversal data
- **Gap filling:** `EvoTreeGenerateIntermediate()` inserts intermediate species when gaps exceed an acceptable threshold
- Uses **Stack** and **Queue** ADTs (linked-list based) for path queries

## EvoTree API Overview
### Core Operations

| Function | Signature | Description | Time Complexity |
|--------|-----------|-------------|-----------------|
| `EvoTreeNew` | `EvoTree EvoTreeNew(void)` | Creates a new empty evolutionary tree. | **O(1)** |
| `EvoTreeFree` | `void EvoTreeFree(EvoTree et)` | Frees all memory allocated to the evolutionary tree. | **O(n)** |
| `EvoTreeInsert` | `void EvoTreeInsert(EvoTree et, int number, char *name)` | Inserts a new species into the tree. Does nothing if the species number already exists. | **O(h)** |
| `EvoTreeNumSpecies` | `int EvoTreeNumSpecies(EvoTree et)` | Returns the total number of species in the tree. | **O(1)** |
| `EvoTreePrint` | `void EvoTreePrint(EvoTree et, FILE *out)` | Prints all species in ascending order of species number. | **O(n)** |

---

### Species Classification & Queries

| Function | Signature | Description | Time Complexity |
|--------|-----------|-------------|-----------------|
| `EvoTreeFind` | `char *EvoTreeFind(EvoTree et, int speciesNumber)` | Returns the species name, or `"undefined"` if not found. | **O(h)** |
| `EvoTreeHistory` | `int EvoTreeHistory(EvoTree et, int speciesNumber, Species history[])` | Stores the evolutionary history of a species (most recent → least recent). | **O(h)** |
| `EvoTreeRelatives` | `int EvoTreeRelatives(EvoTree et, int speciesNumber, int width, Species relatives[])` | Stores all relatives within a numeric range in ascending order. | **O(n)** |

---

### Ancestry & Evolution Paths

| Function | Signature | Description |
|--------|-----------|-------------|
| `EvoTreeCommonAncestor` | `Species EvoTreeCommonAncestor(EvoTree et, int speciesNumbers[], int size)` | Returns the most recent common ancestor of all valid species numbers. |
| `EvoTreeEvolutionPath` | `int EvoTreeEvolutionPath(EvoTree et, int speciesNumberA, int speciesNumberB, Species path[])` | Stores the path connecting two species via their common ancestor. |

---

### Notes
- `n` = number of species in the tree  
- `h` = height of the BST  
- Species numbers are unique and determine tree structure  
- All output ordering is **ascending by species number**

## Build & Run
This project includes a `Makefile`.

```sh
make
./evotree
