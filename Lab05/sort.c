/*
 * File: sort.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Modified on February 7, 2016, 4:42 PM
 */

// Heap size 1024 required!

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "LinkedList.h"

// **** Set any macros or preprocessor directives here ****
// #define LINKED_LIST_TESTING

// **** Declare any data types here ****

// **** Define any module-level, global, or external variables here ****
static char *dog = "dog";
static char *pig1 = "pig";
static char *pig2 = "pig";
static char *cow = "cow";
static char *cat = "cat";
static char *turtle = "turtle";
static char *bird = "bird";
static char *crab = "crab";

// **** Declare any function prototypes here ****
int InitializeUnsortedWordList(ListItem **unsortedWordList);
int UnsortedWordCount(ListItem *list, int *wordCount);
int SortedWordCount(ListItem *list, int *wordCount);

int main()
{
    BOARD_Init();

    /**
     * ----------------------- STARTER CODE --------------------------
     * This segment of code obtains an unsorted word list, prints it out,
     * and displays the number of occurrences of each word in the list as
     * described by UnsortedWordCount() below.
     */
    // Initialize an unsorted word list.
    ListItem *unsortedWordList = NULL;
    if (!InitializeUnsortedWordList(&unsortedWordList) || !unsortedWordList) {
        printf("ERROR: Failed to initialize word list\n");
        while (1);
    }

    // Print the list
    LinkedListPrint(unsortedWordList);

    // Get the word counts for every string in the list
    int g = LinkedListSize(unsortedWordList);
    int wordCount[g];

    // Print the word count results
    unsortedWordList = LinkedListGetFirst(unsortedWordList);
    if (UnsortedWordCount(unsortedWordList, wordCount)) {
        printf("[%d, %d, %d, %d, %d, %d, %d, %d, %d, %d]\n",
                wordCount[0], wordCount[1], wordCount[2],
                wordCount[3], wordCount[4], wordCount[5],
                wordCount[6], wordCount[7], wordCount[8],
                wordCount[9]);
    } else {
        printf("ERROR\n");
    }
    printf("\n");

    int h = LinkedListSize(unsortedWordList);
    int wordCount2[h];

    if (SortedWordCount(unsortedWordList, wordCount2)) {
        printf("[%d, %d, %d, %d, %d, %d, %d, %d, %d, %d]\n",
                wordCount2[0], wordCount2[1], wordCount2[2],
                wordCount2[3], wordCount2[4], wordCount2[5],
                wordCount2[6], wordCount2[7], wordCount2[8],
                wordCount2[9]);
    } else {
        printf("ERROR\n");
    }
    printf("\n");

    /******************************** Your custom code goes below here ********************************/
    /* 
    UNIT TESTING
     * 
    // Test LinkedListNew()
    ListItem *i = LinkedListNew(bird);
    if (!i) {
        printf("ERROR LinkedListNew\n");
    } else {
        printf("PASS LinkedListNew\n");
    }


    // Test LinkedListCreateAfter()
    ListItem *j = LinkedListCreateAfter(i, dog);
    if (!j) {
        printf("ERROR LinkedListCreateAfter (1/3)\n");
    } else {
        printf("PASS LinkedListCreateAfter (1/3)\n");
    }

    ListItem *k = LinkedListCreateAfter(j, pig2);
    if (!k) {
        printf("ERROR LinkedListCreateAfter (2/3)\n");
    } else {
        printf("PASS LinkedListCreateAfter (2/3)\n");
    }

    ListItem *l = LinkedListCreateAfter(k, turtle);
    if (!l) {
        printf("ERROR LinkedListCreateAfter (3/3)\n");
    } else {
        printf("PASS LinkedListCreateAfter (3/3)\n");
    }


    // Test LinkedListSize()
    printf("Size of list: %d\n", LinkedListSize(i));
    printf("Size of list: %d\n", LinkedListSize(j));
    printf("Size of list: %d\n", LinkedListSize(k));
    printf("Size of list: %d\n", LinkedListSize(l));


    // Test LinkedListSwapData(), LinkedListSort(), and LinkedListPrint()
    LinkedListSwapData(i, k);
    LinkedListPrint(i);
    LinkedListSwapData(l, j);
    LinkedListPrint(j);
    LinkedListSwapData(k, i);
    LinkedListPrint(k);
    LinkedListSort(i);
    LinkedListPrint(l);

    // Test LinkedListGetFirst()
    if (!LinkedListGetFirst(j)) {
        printf("ERROR LinkedListGetFirst (1/4)\n");
    } else {
        printf("PASS LinkedListGetFirst (1/4)\n");
    }

    if (!LinkedListGetFirst(k)) {
        printf("ERROR LinkedListGetFirst (2/4)\n");
    } else {
        printf("PASS LinkedListGetFirst (2/4)\n");
    }


    // Test LinkedListRemove() and LinkedListGetFirst()
    if (!LinkedListRemove(k)) {
        printf("ERROR LinkedListRemove (1/2)\n");
    } else {
        printf("PASS LinktedListRemove (1/2)\n");
    }

    printf("Size of list: %d\n", LinkedListSize(i));
    printf("Size of list: %d\n", LinkedListSize(j));
    printf("Size of list: %d\n", LinkedListSize(l));

    LinkedListPrint(i);
    LinkedListPrint(j);
    LinkedListSort(j);
    LinkedListPrint(l);

    if (!LinkedListGetFirst(j)) {
        printf("ERROR LinkedListGetFirst (3/4)\n");
    } else {
        printf("PASS LinkedListGetFirst (3/4)\n");
    }

    if (!LinkedListRemove(j)) {
        printf("ERROR LinkedListRemove (2/2)\n");
    } else {
        printf("PASS LinkedListRemove (2/2)\n");
    }

    printf("Size of list: %d\n", LinkedListSize(i));
    printf("Size of list: %d\n", LinkedListSize(l));

    LinkedListPrint(i);
    LinkedListPrint(l);

    if (!LinkedListGetFirst(l)) {
        printf("ERROR LinkedListGetFirst (4/4)\n");
    } else {
        printf("PASS LinkedListGetFirst (4/4)\n");
    }
     * 
     */



    /******************************** Your custom code goes above here ********************************/

    // You can never return from main() in an embedded system (one that lacks an operating system).
    // This will result in the processor restarting, which is almost certainly not what you want!
    while (1);
}

#ifndef LINKED_LIST_TESTING

/**
 * This functions takes in the head of an unsorted list of words, and an array to store the number
 * of occurrences of each word.  The first time a word appears, the number of occurrences of that
 * word should be stored in the corresponding position in the array.  For repeated words the
 * negative of the number of occurrences should be stored.  For example, a list of the following
 * words should return an array with the following values:
 *     {"hair", "comb", "brush", "brush"} -> {1, 1, 2, -2}
 *
 * NULL values are treated as non-strings, so they always count as 0. Every other string is treated
 * as regular strings that can have multiple occurrences, including the empty string, as the
 * following output demonstrates.
 *     {NULL, "platypus", "giraffe", "", "corvid", "slug", "", NULL, "platypus", "platypus"} ->
 *     [0   , 3         , 1        , 2 , 1       , 1     , -2, 0   , -3        , -3}
 *
 * NOTE: This function assumes that wordCount is the same length as list.
 * @param list A pointer to the head of a doubly-linked list containing unsorted words.
 * @param wordCount An array of integers. The output of this function is stored here. It must be
 *                  at least as big as the linked list pointed to be `list` is.
 * @return Either SUCCESS or STANDARD_ERROR if the head of the doubly-linked list isn't passed.
 */
int UnsortedWordCount(ListItem *list, int *wordCount)
{
    // Make sure the head of the list was given.
    if (list->previousItem != NULL) {
        return STANDARD_ERROR;
    }

    char *word1;
    char *word2;
    char *repCheck;
    ListItem *item1 = list;
    ListItem *item2;
    int temp, firstOccurencePos;
    int repetitionFlag = 0;
    int i, j;

    i = 0;
    while (item1 != NULL) {
        // Count first occurrence
        j = 1;
        // Ignore NULL words
        word1 = (char *) item1->data;
        if (word1 == NULL) {
            wordCount[i++] = 0;
            item1 = item1->nextItem;
            continue;
        }

        // Check previous occurrences
        temp = i;
        item2 = item1->previousItem;
        while (item2 != NULL) {
            --temp;
            repCheck = (char *) item2->data;
            if (repCheck != NULL && strcmp(repCheck, word1) == 0) {
                repetitionFlag = 1;
                firstOccurencePos = temp;
            }
            item2 = item2->previousItem;
        }

        // Count new occurrences
        if (repetitionFlag == 0) {
            item2 = LinkedListGetFirst(list);
            while (item2 != NULL) {
                word2 = (char *) item2->data;
                if (word2 != NULL && item1 != item2 && strcmp(word1, word2) == 0) {
                    ++j;
                }
                item2 = item2->nextItem;
            }
            wordCount[i++] = j;
        } else {
            wordCount[i++] = -wordCount[firstOccurencePos];
            repetitionFlag = 0;
        }
        item1 = item1->nextItem;
    }
    return SUCCESS;
}

/**
 * This function initializes a list of ListItems for use when testing the LinkedList implementation
 * and word count algorithms.
 *
 * Usage:
 * ListItem *newList;
 * InitializeUnsortedWordList(&newList);
 *
 * @param unsortedWordList[out] Where to store the pointer to the head of the list.
 * @return SUCCESS if it succeeds, STANDARD_ERROR if it fails to allocate necessary memory.
 */
int InitializeUnsortedWordList(ListItem **unsortedWordList)
{
    ListItem *tmp = LinkedListNew(crab);
    if (!tmp) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, pig1)) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, cow)) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, NULL)) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, dog)) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, cow)) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, bird)) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, pig2)) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, cat)) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, turtle)) {
        return STANDARD_ERROR;
    }
    *unsortedWordList = tmp;
    return SUCCESS;
}

/**
 * SortedWordCount() takes in the head of a sorted list of words arranged lexicographically, and an
 * array to store the number of occurrences of each word.  The first time a word appears, the number
 * of occurrences of that word should be stored in the corresponding position in the array.  For
 * duplicate words a negative value of the number of occurrences should be stored.  For example, a
 * list of the following words should return an array with the following values:
 *     {"brush", "brush", "comb", "hair"} -> {2, -2, 1, 1}
 *
 * NULL values are treated as non-strings, so they always count as 0. Every other string is treated
 * as regular strings that can have multiple occurrences, including the empty string, as the
 * following output demonstrates.
 *     {NULL, "platypus", "giraffe", "", "corvid", "slug", "", NULL, "platypus", "platypus"} ->
 *     [0   , 3         , 1        , 2 , 1       , 1     , -2, 0   , -3        , -3}
 *
 * NOTE: This function assumes that wordCount is the same length as list.
 * @param list A pointer to the head of a doubly-linked list containing sorted words.
 * @param wordCount An array of integers. The output of this function is stored here. It must be
 *                  at least as big as the linked list pointed to be `list` is.
 * @return Either SUCCESS or STANDARD_ERROR if the head of the doubly-linked list isn't passed.
 */
int SortedWordCount(ListItem *list, int *wordCount)
{
    LinkedListSort(list);
    int x = UnsortedWordCount(list, wordCount);
    return x;
}

#endif
