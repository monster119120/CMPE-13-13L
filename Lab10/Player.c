/*
 * File: Player.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Modified on March 13, 2016, 9:50 AM
 */

//CMPE13 Support Library
#include "Player.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// **** Set any macros or preprocessor directives here ****

#define ZERO 0
#define ONE 1

//Item Inventory array

static uint8_t item_inventory [INVENTORY_SIZE + ONE] = {ZERO,ZERO,ZERO,ZERO};
static uint8_t loop_ctr;




/**
 * Adds the specified item to the player's inventory if the inventory isn't full.
 * @param item The item number to be stored: valid values are 0-255.
 * @return SUCCESS if the item was added, STANDARD_ERRROR if the item couldn't be added.
 */
int AddToInventory(uint8_t item)
{
    for (loop_ctr = ZERO; loop_ctr < INVENTORY_SIZE; loop_ctr++)
    {
        if (item_inventory[loop_ctr] == ZERO) {
            //move item to item_inventory array
            item_inventory [loop_ctr] = item;
            return SUCCESS;
        }
    }
    return STANDARD_ERROR;

}

/**
 * Check if the given item exists in the player's inventory.
 * @param item The number of the item to be searched for: valid values are 0-255.
 * @return SUCCESS if it was found or STANDARD_ERROR if it wasn't.
 */
int FindInInventory(uint8_t item)
{
    for (loop_ctr = ZERO; loop_ctr < INVENTORY_SIZE; loop_ctr++)
    {
        if (item_inventory[loop_ctr] == item) {
            //item is in item_inventory array
            return SUCCESS;
        }
    }
    return STANDARD_ERROR;
}
