/*
 * File: LinkedList.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Created on February 7, 2016, 4:41 PM
 */

// ***Include libraries here ***
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "LinkedList.h"

// **** Define global, module-level, or external variables here ****
#define STANDARD_ERROR 0;
#define SUCCESS 1;

/**
 * This function starts a new linked list. Given an allocated pointer to data it will return a
 * pointer for a malloc()ed ListItem struct. If malloc() fails for any reason, then this function
 * returns NULL otherwise it should return a pointer to this new list item. data can be NULL.
 *
 * @param data The data to be stored in the first ListItem in this new list. Can be any valid 
 *             pointer value.
 * @return A pointer to the malloc()'d ListItem. May be NULL if an error occured.
 */
ListItem *LinkedListNew(char *data)
{
    ListItem *listptr = malloc(sizeof (ListItem));
    if (!listptr) {
        return NULL;
    } else {
        listptr->data = data;
        listptr->previousItem = 0;
        listptr->nextItem = 0;

        return listptr;
    }
}

/**
 * This function will remove a list item from the linked list and free() the memory that the
 * ListItem struct was using. It doesn't, however, free() the data pointer and instead returns it
 * so that the calling code can manage it.  If passed a pointer to NULL, LinkedListRemove() should
 * return NULL to signal an error.
 *
 * @param item The ListItem to remove from the list.
 * @return The data pointer from the removed item. May be NULL.
 */
char *LinkedListRemove(ListItem *item)
{
    ListItem *remove_item = item;

    if (remove_item) {
        ListItem *previous_item = remove_item->previousItem;

        ListItem *prev_a = remove_item->previousItem;
        ListItem *next_c = remove_item->nextItem;

        ListItem *next_item = remove_item->nextItem;

        previous_item->nextItem = next_c;
        next_item->previousItem = prev_a;

        free(remove_item);
        return "1";
    } else {
        return NULL;
    }
}

/**
 * This function allocates a new ListItem containing data and inserts it into the list directly
 * after item. It rearranges the pointers of other elements in the list to make this happen. If
 * passed a NULL item, InsertAfter() should still create a new ListItem, just with no previousItem.
 * It returns NULL if it can't malloc() a new ListItem, otherwise it returns a pointer to the new
 * item. The data parameter is also allowed to be NULL.
 *
 * @param item The ListItem that will be before the newly-created ListItem.
 * @param data The data the new ListItem will point to.
 * @return A pointer to the newly-malloc()'d ListItem.
 */
ListItem *LinkedListCreateAfter(ListItem *item, char *data)
{
    ListItem *listptr = malloc(sizeof (ListItem));
    if (!listptr) {
        return NULL;
    } else {
        ListItem *update_item = item;
        if (update_item->nextItem != NULL) {
            ListItem *move_item = update_item->nextItem;
            move_item->previousItem = listptr;
        }

        listptr->data = data;
        listptr->previousItem = item;
        listptr->nextItem = update_item->nextItem;

        update_item->nextItem = listptr;

        return listptr;
    }
}

/**
 * This function returns the head of a list given some element in the list. If it is passed NULL,
 * it will just return NULL. If given the head of the list it will just return the pointer to the
 * head anyways for consistency.
 *
 * @param list An element in a list.
 * @return The first element in the list. Or NULL if provided an invalid list.
 */
ListItem *LinkedListGetFirst(ListItem *list)
{
    ListItem *head_item = list;
    if (head_item == NULL) {
        return STANDARD_ERROR;
    } else {
        if (head_item->previousItem == NULL) {
            return head_item;
        } else {
            while (head_item->previousItem != NULL) {
                head_item = head_item->previousItem;
            }
            return head_item;
        }
    }
}

/**
 * This function returns the total size of the linked list. This means that even if it is passed a
 * ListItem that is not at the head of the list, it should still return the total number of
 * ListItems in the list. A NULL argument will result in 0 being returned.
 *
    }
 * @param list An item in the list to be sized.
 * @return The number of ListItems in the list (0 if `list` was NULL).
 */
int LinkedListSize(ListItem *list)
{
    ListItem *next_item = LinkedListGetFirst(list);
    if (!next_item) {
        return STANDARD_ERROR;
    } else {
        int ctr = 0;
        while (next_item) {
            next_item = next_item->nextItem;
            ctr++;
        }
        return ctr;
    }
}

/**
 * LinkedListPrint() prints out the complete list to stdout. This function prints out the given
 * list, starting at the head if the provided pointer is not the head of the list, like "[STRING1,
 * STRING2, ... ]" If LinkedListPrint() is called with a NULL list it does nothing, returning
 * STANDARD_ERROR. If passed a valid pointer, prints the list and returns SUCCESS.
 *
 * @param list Any element in the list to print.
 * @return SUCCESS or STANDARD_ERROR if passed NULL pointers.
 */
int LinkedListPrint(ListItem *list)
{
    ListItem *next_item = LinkedListGetFirst(list);
    if (next_item) {
        while (next_item) {
            printf("%s ", next_item->data);
            printf(" ");
            next_item = next_item->nextItem;
        }
        printf("\n");
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/**
 * LinkedListSwapData() switches the data pointers of the two provided ListItems. This is most
 * useful when trying to reorder ListItems but when you want to preserve their location. It is used
 * within LinkedListSort() for swapping items, but probably isn't too useful otherwise. This
 * function should return STANDARD_ERROR if either arguments are NULL, otherwise it should return
 * SUCCESS. If one or both of the data pointers are NULL in the given ListItems, it still does
 * perform the swap and returns SUCCESS.
 *
 * @param firstItem One of the items whose data will be swapped.
 * @param secondItem Another item whose data will be swapped.
 * @return SUCCESS if the swap worked or STANDARD_ERROR if it failed.
 */
int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem)
{
    if ((!firstItem) || (!secondItem)) {
        return STANDARD_ERROR;
    } else {
        ListItem *temp1 = firstItem;
        ListItem *temp2 = secondItem;

        firstItem->data = temp2->data;
        secondItem->data = temp1->data;

        return SUCCESS;
    }
}

/**
 * LinkedListSort() performs a selection sort on list to sort the elements into descending order. It
 * makes no guarantees of the addresses of the list items after sorting, so any ListItem referenced
 * before a call to LinkedListSort() and after may contain different data as only the data pointers
 * for the ListItems in the list are swapped. This function sorts the strings in ascending order
 * first by size (with NULL data pointers counting as 0-length strings) and then alphabetically
 * ascending order. So the list [dog, cat, duck, goat, NULL] will be sorted to [NULL, cat, dog,
 * duck, goat]. LinkedListSort() returns SUCCESS if sorting was possible. If passed a NULL pointer
 * for either argument, it will do nothing and return STANDARD_ERROR.
 *
 * @param list Any element in the list to sort.
 * @return SUCCESS if successful or STANDARD_ERROR is passed NULL pointers.
 */
int LinkedListSort(ListItem *list)
{
    // get head first
    ListItem *head_item = LinkedListGetFirst(list);
    int length = LinkedListSize(list);

    if (head_item) {
        ListItem *outer_item = head_item;
        ListItem *inner_item = head_item->nextItem;
        ListItem *sort_outer;
        ListItem *sort_inner;
        ListItem *next_o = outer_item->nextItem;
        ListItem *next_i = inner_item->nextItem;
        int i = 0;
        int j = 0;

        for (i = 0; i <= (length - 2); i++) {
            next_o = outer_item->nextItem;
            for (j = i + 1; j <= (length - 1); j++) {
                next_i = inner_item->nextItem;
                int compare;
                if ((inner_item->data == NULL) || (outer_item->data == NULL)) {
                    compare = -1;
                } else {
                    compare = strcmp(inner_item->data, outer_item->data);
                }
                if (compare < 0) {
                    sort_outer = outer_item;
                    sort_inner = inner_item;
                    LinkedListSwapData(sort_inner, sort_outer);
                    outer_item = sort_inner;
                    inner_item = sort_outer;
                }
                inner_item = next_i;
            }
            outer_item = next_o;
        }
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}
