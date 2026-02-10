// COMP2521 - Assignment 1
// Tests for the Evolutionary Tree ADT

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EvoTree.h"

void testEvoTreeInsert(void);
void testEvoTreeNumSpecies(void);
void testEvoTreePrint(void);
void checkFileContents(FILE *file, char *expectedContents);
void testEvoTreeFind(void);
void testEvoTreeHistory(void);
void testEvoTreeRelatives(void);
void testEvoTreeCommonAncestor(void);
void testEvoTreeEvolutionPath(void);
void testEvoTreeConstruct(void);
void testEvoTreeGenerateIntermediate1(void);
void testEvoTreeGenerateIntermediate2(void);

int main(int argc, char **argv) {
    testEvoTreeInsert();
    testEvoTreeNumSpecies();
    testEvoTreePrint();
    testEvoTreeFind();
    testEvoTreeHistory();
    testEvoTreeRelatives();
    testEvoTreeCommonAncestor();
    testEvoTreeEvolutionPath();
    testEvoTreeConstruct();
    testEvoTreeGenerateIntermediate1();
    testEvoTreeGenerateIntermediate2();
}

////////////////////////////////////////////////////////////////////////

void testEvoTreeInsert(void) {
    // NOTE: EvoTreeInsert can't be tested on its own unless we directly
    //       access the internal representation of the ADT

    EvoTree et = EvoTreeNew();
    EvoTreeInsert(et, 20, "lavezar");
    EvoTreeInsert(et, 15, "lachari");
    EvoTreeInsert(et, 10, "charmander");
    EvoTreeInsert(et, 30, "malizar");
    EvoTreeInsert(et, 25, "salandit");

    assert(EvoTreeNumSpecies(et) == 5);

    EvoTreeFree(et);
}

void testEvoTreeNumSpecies(void) {
    // printf("\ntestEvoTreeNumSpecies\n");

    EvoTree et = EvoTreeNew();

    EvoTreeInsert(et, 20, "lavezar");
    EvoTreeInsert(et, 15, "lachari");
    EvoTreeInsert(et, 10, "charmander");
    EvoTreeInsert(et, 30, "malizar");
    // Debug: 
    // int result1 = EvoTreeNumSpecies(et);
    // printf("result 1: %d\n", result1);

    assert(EvoTreeNumSpecies(et) == 4);

    EvoTreeInsert(et, 25, "salandit");
    EvoTreeInsert(et, 30, "lizarid");   // duplicated number
    // Debug: 
    // int result2 = EvoTreeNumSpecies(et);
    // printf("result 2: %d\n", result2);

    assert(EvoTreeNumSpecies(et) == 5);

    EvoTreeFree(et);
}

void testEvoTreePrint(void) {
    // printf("\ntestEvoTreePrint\n");
    EvoTree et = EvoTreeNew();

    EvoTreeInsert(et, 20, "lavezar");
    EvoTreeInsert(et, 15, "lachari");
    EvoTreeInsert(et, 10, "charmander");
    EvoTreeInsert(et, 30, "malizar");

    FILE *out = tmpfile();
    EvoTreePrint(et, out);
    
    checkFileContents(out, "{(10, charmander), (15, lachari), (20, lavezar), (30, malizar)}");
    fclose(out);

    EvoTreeFree(et);
}

// /**
//  * This function checks if the contents of a file matches the expected
//  * contents. If the contents do not match, the program will exit with an
//  * assertion error.
//  */
void checkFileContents(FILE *file, char *expectedContents) {
    fflush(file);
    fseek(file, 0, SEEK_SET);
    char *line = NULL;
    size_t n = 0;
    getline(&line, &n, file);
    assert(strcmp(line, expectedContents) == 0);
    free(line);
}

////////////////////////////////////////////////////////////////////////

void testEvoTreeFind(void) {
    // printf("\ntestEvoTreeFind\n");
    EvoTree et = EvoTreeNew();

    EvoTreeInsert(et, 20, "lavezar");
    EvoTreeInsert(et, 15, "lachari");
    EvoTreeInsert(et, 10, "charmander");
    EvoTreeInsert(et, 30, "malizar");

    // Debug: my own test
    // char *string1 = EvoTreeFind(et, 10);
    // char *string2 = EvoTreeFind(et, 15);
    // char *string3 = EvoTreeFind(et, 20);
    // char *string4 = EvoTreeFind(et, 25);
    // char *string5 = EvoTreeFind(et, 30);
 
    // printf("string 1: %s\n", string1); // charmander
    // printf("string 2: %s\n", string2); // lachari
    // printf("string 3: %s\n", string3); // lavezar
    // printf("string 4: %s\n", string4); // undefined
    // printf("string 5: %s\n", string5); // malizar

     // provided test
    assert(strcmp(EvoTreeFind(et, 10), "charmander") == 0);
    assert(strcmp(EvoTreeFind(et, 15), "lachari") == 0);
    assert(strcmp(EvoTreeFind(et, 20), "lavezar") == 0);
    assert(strcmp(EvoTreeFind(et, 25), "undefined") == 0);
    assert(strcmp(EvoTreeFind(et, 30), "malizar") == 0);

    EvoTreeFree(et);
}

void testEvoTreeHistory(void) {
    EvoTree et = EvoTreeNew();

    EvoTreeInsert(et, 20, "lavezar");
    EvoTreeInsert(et, 15, "lachari");
    EvoTreeInsert(et, 10, "charmander");
    EvoTreeInsert(et, 30, "malizar");
    EvoTreeInsert(et, 25, "salandit");

    Species ancestors[5];
    // Debug: my own test
    int count = EvoTreeHistory(et, 10, ancestors);
    // printf("count is %d\n", count); // count = 3

    assert(count == 3);
    assert(ancestors[0].number == 10);
    assert(strcmp(ancestors[0].name, "charmander") == 0);
    assert(ancestors[1].number == 15);
    assert(strcmp(ancestors[1].name, "lachari") == 0);
    assert(ancestors[2].number == 20);
    assert(strcmp(ancestors[2].name, "lavezar") == 0);

    // Debug: my own test
    int count2 = EvoTreeHistory(et, 30, ancestors);
    // printf("count is %d\n", count2); // count = 2

    assert(count2 == 2);
    assert(ancestors[0].number == 30);
    assert(strcmp(ancestors[0].name, "malizar") == 0);
    assert(ancestors[1].number == 20);
    assert(strcmp(ancestors[1].name, "lavezar") == 0);

    EvoTreeFree(et);
}

void testEvoTreeRelatives(void) {
    // printf("\ntestEvoTreeRelatives\n");
    EvoTree et = EvoTreeNew();

    EvoTreeInsert(et, 70, "liskopo");
    EvoTreeInsert(et, 80, "tokapants");
    EvoTreeInsert(et, 85, "scraggy");
    EvoTreeInsert(et, 67, "lagorella");
    EvoTreeInsert(et, 65, "helioptile");

    Species relatives[5];
    int count = EvoTreeRelatives(et, 70, 10, relatives);
    // Debug: my own test
    // printf("count for relatives: %d\n", count);

    // Debug: my own test
    // char *relative_name0 = relatives[0].name;
    // int relative_num0 = relatives[0].number;
    // char *relative_name1 = relatives[1].name;
    // int relative_num1 = relatives[1].number;
    // char *relative_name2 = relatives[2].name;
    // int relative_num2= relatives[2].number;
 
    // Debug: my own test
    // printf("relative_name 1: %s\n", relative_name0); // helioptile
    // printf("relative_num 1: %d\n", relative_num0); // 65
    // printf("relative_name 2: %s\n", relative_name1); // lagorella
    // printf("relative_num 2: %d\n", relative_num1); // 67
    // printf("relative_name 2: %s\n", relative_name2); // tokapants
    // printf("relative_num 2: %d\n", relative_num2); // 80

    // Provided 
    assert(count == 3);
    assert(relatives[0].number == 65);
    assert(strcmp(relatives[0].name, "helioptile") == 0);
    assert(relatives[1].number == 67);
    assert(strcmp(relatives[1].name, "lagorella") == 0);
    assert(relatives[2].number == 80);
    assert(strcmp(relatives[2].name, "tokapants") == 0);

    EvoTreeFree(et);
}

// ////////////////////////////////////////////////////////////////////////

void testEvoTreeCommonAncestor(void) {

    // printf("\ntestEvoTreeCommonAncestor\n");

    EvoTree et = EvoTreeNew();

    EvoTreeInsert(et, 50, "lizarid");
    EvoTreeInsert(et, 20, "lavezar");
    EvoTreeInsert(et, 15, "lachari");
    EvoTreeInsert(et, 10, "charmander");
    EvoTreeInsert(et, 30, "malizar");
    EvoTreeInsert(et, 25, "salandit");
    EvoTreeInsert(et, 70, "liskopo");
    EvoTreeInsert(et, 80, "tokapants");
    EvoTreeInsert(et, 85, "scraggy");
    EvoTreeInsert(et, 67, "lagorella");
    EvoTreeInsert(et, 65, "helioptile");

    Species ancestor;
    int speciesNumbers[5];

    // printf("Case 1:\n");
    speciesNumbers[0] = 15;
    speciesNumbers[1] = 25;
    ancestor = EvoTreeCommonAncestor(et, speciesNumbers, 2);
    // printf("ancestor name: %s\n", ancestor.name);   // 20
    // printf("ancestor number: %d\n", ancestor.number);   // lavezar
    assert(ancestor.number == 20);
    assert(strcmp(ancestor.name, "lavezar") == 0);
    
    // printf("Case 2:\n");
    speciesNumbers[2] = 65;
    // debug
    ancestor = EvoTreeCommonAncestor(et, speciesNumbers, 3);
    // printf("ancestor name: %s\n", ancestor.name);   // 50
    // printf("ancestor number: %d\n", ancestor.number);   // lizarid
    ancestor = EvoTreeCommonAncestor(et, speciesNumbers, 3);
    assert(ancestor.number == 50);
    assert(strcmp(ancestor.name, "lizarid") == 0);

    // printf("Case 3:\n");
    // speciesNumbers[0] = 44;
    // speciesNumbers[1] = 61;
    // speciesNumbers[2] = 85;
    // speciesNumbers[3] = 91;
    // ancestor = EvoTreeCommonAncestor(et, speciesNumbers, 4);
    // printf("ancestor name: %s\n", ancestor.name);  
    // printf("ancestor number: %d\n", ancestor.number);   

    speciesNumbers[2] = 55;
    ancestor = EvoTreeCommonAncestor(et, speciesNumbers, 3);
    // printf("ancestor name: %s\n", ancestor.name);  
    assert(ancestor.number == 20);
    assert(strcmp(ancestor.name, "lavezar") == 0);

    EvoTreeFree(et);
}

void testEvoTreeEvolutionPath(void) {
    // printf("\ntestEvoTreeEvolutionPath\n");
    EvoTree et = EvoTreeNew();

    EvoTreeInsert(et, 50, "lizarid");
    EvoTreeInsert(et, 20, "lavezar");
    EvoTreeInsert(et, 15, "lachari");
    EvoTreeInsert(et, 10, "charmander");
    EvoTreeInsert(et, 30, "malizar");
    EvoTreeInsert(et, 25, "salandit");
    EvoTreeInsert(et, 70, "liskopo");
    EvoTreeInsert(et, 80, "tokapants");
    EvoTreeInsert(et, 85, "scraggy");
    EvoTreeInsert(et, 67, "lagorella");
    EvoTreeInsert(et, 65, "helioptile");

    Species path[11];
    int count = EvoTreeEvolutionPath(et, 15, 65, path);

    // printf("(%d, %s)\n", path[0].number, path[0].name);
    // printf("(%d, %s)\n", path[1].number, path[1].name);
    // printf("(%d, %s)\n", path[2].number, path[2].name);
    // printf("(%d, %s)\n", path[3].number, path[3].name);
    // printf("(%d, %s)\n", path[4].number, path[4].name);
    // printf("(%d, %s)\n", path[5].number, path[5].name);

    // printf("\nLength of path is: %d\n", count);
    assert(count == 6);
    assert(path[0].number == 15);
    assert(strcmp(path[0].name, "lachari") == 0);
    assert(path[1].number == 20);
    assert(strcmp(path[1].name, "lavezar") == 0);
    assert(path[2].number == 50);
    assert(strcmp(path[2].name, "lizarid") == 0);
    assert(path[3].number == 70);
    assert(strcmp(path[3].name, "liskopo") == 0);
    assert(path[4].number == 67);
    assert(strcmp(path[4].name, "lagorella") == 0);
    assert(path[5].number == 65);
    assert(strcmp(path[5].name, "helioptile") == 0);

    EvoTreeFree(et);
    printf("\n \n");
    EvoTree et2 = EvoTreeNew();
    EvoTreeInsert(et2, 166, "a");
    EvoTreeInsert(et2, 152, "a");
    EvoTreeInsert(et2, 163, "a");
    EvoTreeInsert(et2, 160, "a");

    Species path2[3];
    EvoTreeEvolutionPath(et2, 160, 152, path2);
    for (int i = 0; i < 3; i++) {
        printf("%d ", path2[i].number);
    }
}

// ////////////////////////////////////////////////////////////////////////

void testEvoTreeConstruct(void) {

    // printf("\ntestEvoTreeConstruct\n");

    // NOTE: EvoTreeConstruct will be tested more directly by
    //       accessing the internal representation of the ADT

    // Species data[5] = {
    //     {10, "charmander"},
    //     {15, "lachari"},
    //     {25, "salandit"},
    //     {30, "malizar"},
    //     {20, "lavezar"}
    // };
    Species data[10] = {
        {4, "charmander"},
        {7, "lachari"},
        {10, "malizar"},
        {8, "lavezar"},
        {19, "helioptile"},
        {17, "lagorella"},
        {28, "scraggy"},
        {25, "tokapants"},
        {21, "liskopo"},
        {15, "lizarid"}
    };

    EvoTree et = EvoTreeConstruct(data, 10);

    FILE *out = tmpfile();
    EvoTreePrint(et, out);
    checkFileContents(out, "{(4, charmander), (7, lachari), (8, lavezar), (10, malizar), (15, lizarid), (17, lagorella), (19, helioptile), (21, liskopo), (25, tokapants), (28, scraggy)}");
    // checkFileContents(out, "{(10, charmander), (15, lachari), (20, lavezar), (25, salandit), (30, malizar)}");
    fclose(out);

    assert(EvoTreeNumSpecies(et) == 10);

    EvoTreeFree(et);
}

// ////////////////////////////////////////////////////////////////////////

void testEvoTreeGenerateIntermediate1(void) {

    // printf("\ntestEvoTreeGenerateIntermediate1\n");

    EvoTree et = EvoTreeNew();

    EvoTreeInsert(et, 30, "lavezar");
    EvoTreeInsert(et, 20, "lachari");
    EvoTreeInsert(et, 10, "charmander");
    EvoTreeInsert(et, 45, "malizar");
    EvoTreeInsert(et, 40, "salandit");

    int count = EvoTreeGenerateIntermediate(et, 8);
    // printf("count is %d \n", count);

    assert(count == 3);

    // check that "lavezarlachari" is inserted between "lavezar" and "lachari"
    int insertedExists1 = 0;
    for (int i = 22; i <= 28; i++) {
        if (strcmp(EvoTreeFind(et, i), "lavezarlachari") == 0) insertedExists1++;
    }
    assert(insertedExists1 == 1);

    // check that "lacharicharmander" is inserted between "lachari" and "charmander"
    int insertedExists2 = 0;
    for (int i = 12; i <= 18; i++) {
        if (strcmp(EvoTreeFind(et, i), "lacharicharmander") == 0) insertedExists2++;
    }
    assert(insertedExists2 == 1);

    // check that "lavezarmalizar" is inserted between "lavezar" and "malizar"
    int insertedExists3 = 0;
    for (int i = 37; i <= 38; i++) {
        if (strcmp(EvoTreeFind(et, i), "lavezarmalizar") == 0) insertedExists3++;
    }
    assert(insertedExists3 == 1);

    assert(EvoTreeNumSpecies(et) == 8);

    EvoTreeFree(et);
}

void testEvoTreeGenerateIntermediate2(void) {
    // printf("\ntestEvoTreeGenerateIntermediate2\n");
    EvoTree et = EvoTreeNew();

    EvoTreeInsert(et, 30, "lavezar");
    EvoTreeInsert(et, 10, "charmander");

    int acceptableGap = 5;

    int countInserted = EvoTreeGenerateIntermediate(et, acceptableGap);
    // printf("2 Inserted %d\n", countInserted);
    assert(countInserted == 3);

    Species ancestors[5];
    int countAncestors = EvoTreeHistory(et, 10, ancestors);
    // printf("countAncestors: %d\n", countAncestors);

    assert(countAncestors == 5);
    assert(ancestors[0].number == 10);
    assert(strcmp(ancestors[0].name, "charmander") == 0);
    assert(strcmp(ancestors[1].name, "lavezarcharmander3") == 0);
    assert(strcmp(ancestors[2].name, "lavezarcharmander2") == 0);
    assert(strcmp(ancestors[3].name, "lavezarcharmander1") == 0);
    assert(ancestors[4].number == 30);
    assert(strcmp(ancestors[4].name, "lavezar") == 0);

    for (int i = 1; i < countAncestors; i++) {
        assert(ancestors[i].number > ancestors[i - 1].number);
        assert(ancestors[i].number - ancestors[i - 1].number <= acceptableGap);
    }

    assert(EvoTreeNumSpecies(et) == 5);

    EvoTreeFree(et);
}

