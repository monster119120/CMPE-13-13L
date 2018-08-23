/*
 * File: Game.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Modified on March 13, 2016, 9:58 AM
 */

// Standard C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// User libraries
#include "Game.h"
#include "Player.h"


;

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// **** Declare any function prototypes here ****

static int loadRoom(void);
static void initStruct(void);

// **** Set any macros or preprocessor directives here ****

#define GAME_MAX_ROOM_EXITS 4
#define GAME_MAX_ROOM_ITEMS 255
#define ROOM_FILENAME "RoomFiles/room%u.txt"

#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3
#define TWENTY 20
#define charNULL '\0'
#define stringNULL "\0"


// **** Declare any data types here ****

static char roomFilename[TWENTY];
static uint8_t item_found;
static uint8_t tempLength;
static uint8_t tempExit;
static uint8_t loop_ctr;
static uint8_t returnRoomExits;

static struct roomDetails
{
    uint8_t roomNum;
    uint8_t fieldLength;
    char Title[GAME_MAX_ROOM_TITLE_LENGTH + ONE];
    uint8_t Item_Requirements[GAME_MAX_ROOM_ITEMS + ONE];
    char Desc[GAME_MAX_ROOM_DESC_LENGTH + ONE];
    uint8_t Item_Contained[GAME_MAX_ROOM_ITEMS + ONE];
    uint8_t Exits[GAME_MAX_ROOM_EXITS + ONE];
} currentRoom;

// **** Define any global or external variables here ****


FILE *fp;

/**
 * These function transitions between rooms. Each call should return SUCCESS if the current room has
 * an exit in the correct direction and the new room was able to be loaded, and STANDARD_ERROR
 * otherwise.
 * @return SUCCESS if the room CAN be navigated to and changing the current room to that new room
 *         succeeded.
 */
// Room Exit arrangement is NESW
int GameGoNorth(void)
{
    if (currentRoom.Exits[ZERO])
    {
        currentRoom.roomNum = currentRoom.Exits[ZERO];
        sprintf(roomFilename,ROOM_FILENAME,currentRoom.roomNum);
        fp = fopen(roomFilename, "rb");
        if (fp == NULL)
        {
            FATAL_ERROR();
        }
        else
        {
            if (loadRoom())
            {
                return SUCCESS;
            }
            else
            {
                return STANDARD_ERROR;
            }
        }
    }
    else
    {
        //not a valid exit
        return SUCCESS;
    }
}


int GameGoEast(void)
{
    if (currentRoom.Exits[ONE])
    {
        currentRoom.roomNum = currentRoom.Exits[ONE];
        sprintf(roomFilename,ROOM_FILENAME,currentRoom.roomNum);
        fp = fopen(roomFilename, "rb");
        if (fp == NULL)
        {
            FATAL_ERROR();
        }
        else
        {
            if (loadRoom())
            {
                return SUCCESS;
            }
            else
            {
                return STANDARD_ERROR;
            }
        }
    }
    else
    {
        //not a valid exit
        return SUCCESS;
    }
}


int GameGoSouth(void)
{
    if (currentRoom.Exits[TWO])
    {
        currentRoom.roomNum = currentRoom.Exits[TWO];
        sprintf(roomFilename,ROOM_FILENAME,currentRoom.roomNum);
        fp = fopen(roomFilename, "rb");
        if (fp == NULL)
        {
            FATAL_ERROR();
        }
        else
        {
            if (loadRoom())
            {
                return SUCCESS;
            }
            else
            {
                return STANDARD_ERROR;
            }
        }
    }
    else
    {
        //not a valid exit
        return SUCCESS;
    }
}


int GameGoWest(void)
{
    if (currentRoom.Exits[THREE])
    {
        currentRoom.roomNum = currentRoom.Exits[THREE];
        sprintf(roomFilename,ROOM_FILENAME,currentRoom.roomNum);
        fp = fopen(roomFilename, "rb");
        if (fp == NULL)
        {
            FATAL_ERROR();
        }
        else
        {
            if (loadRoom())
            {
                return SUCCESS;
            }
            else
            {
                return STANDARD_ERROR;
            }
        }
    }
    else
    {
        //not a valid exit
        return SUCCESS;
    }
}

/**
 * This function sets up anything that needs to happen at the start of the game. This is just
 * setting the current room to STARTING_ROOM and loading it. It should return SUCCESS if it succeeds
 * and STANDARD_ERROR if it doesn't.
 * @return SUCCESS or STANDARD_ERROR
 */
int GameInit(void)
{
    // Start at Room 32
    currentRoom.roomNum = STARTING_ROOM;
    sprintf(roomFilename,ROOM_FILENAME,currentRoom.roomNum);
    fp = fopen(roomFilename, "rb");
    if (fp == NULL)
    {
        return STANDARD_ERROR;
    }
    else
    {
        if (loadRoom())
        {
            return SUCCESS;
        }
        else
        {
            return STANDARD_ERROR;
        }
    }


}

/**
 * Copies the current room title as a NULL-terminated string into the provided character array.
 * Only a NULL-character is copied if there was an error so that the resultant output string
 * length is 0.
 * @param title A character array to copy the room title into. Should be GAME_MAX_ROOM_TITLE_LENGTH+1
 *             in length in order to allow for all possible titles to be copied into it.
 * @return The length of the string stored into `title`. Note that the actual number of chars
 *         written into `title` will be this value + 1 to account for the NULL terminating
 *         character.
 */
int GameGetCurrentRoomTitle(char *title)
{
    if (strncpy(title, currentRoom.Title,(GAME_MAX_ROOM_TITLE_LENGTH + ONE)) == charNULL)
    {
        title = charNULL;
    }
    else
    {
        strncat(title, stringNULL, ONE);
    }
    return strlen(title);

}


/**
 * GetCurrentRoomDescription() copies the description of the current room into the argument desc as
 * a C-style string with a NULL-terminating character. The room description is guaranteed to be less
 * -than-or-equal to GAME_MAX_ROOM_DESC_LENGTH characters, so the provided argument must be at least
 * GAME_MAX_ROOM_DESC_LENGTH + 1 characters long. Only a NULL-character is copied if there was an
 * error so that the resultant output string length is 0.
 * @param desc A character array to copy the room description into.
 * @return The length of the string stored into `desc`. Note that the actual number of chars
 *          written into `desc` will be this value + 1 to account for the NULL terminating
 *          character.
 */
int GameGetCurrentRoomDescription(char *desc)
{
    if (strncpy(desc,currentRoom.Desc, GAME_MAX_ROOM_DESC_LENGTH + ONE) == charNULL)
    {
        desc = charNULL;
    }
    else
    {
        strncat(desc, stringNULL, ONE);
    }
    return strlen(desc);
}


/**
 * This function returns the exits from the current room in the lowest-four bits of the returned
 * uint8 in the order of NORTH, EAST, SOUTH, and WEST such that NORTH is in the MSB and WEST is in
 * the LSB. A bit value of 1 corresponds to there being a valid exit in that direction and a bit
 * value of 0 corresponds to there being no exit in that direction. The GameRoomExitFlags enum
 * provides bit-flags for checking the return value.
 *
 * @see GameRoomExitFlags
 *
 * @return a 4-bit bitfield signifying which exits are available to this room.
 */
// currentRoom.roomExits [N,E,S,W]
//return returnRoomExits 0000NESW
uint8_t GameGetCurrentRoomExits(void)
{

    returnRoomExits = ZERO;

    if (currentRoom.Exits[ZERO] != ZERO)
    {
        // The current room has a north exit.
        returnRoomExits ^= (ONE << THREE);
    }

    if (currentRoom.Exits[ONE] != ZERO)
    {
        // The current room has a east exit.
        returnRoomExits ^= (ONE << TWO);
    }

    if (currentRoom.Exits[TWO] != ZERO)
    {
        // The current room has a south exit.
        returnRoomExits ^= (ONE << ONE);
    }

    if (currentRoom.Exits[THREE] != ZERO)
    {
        // The current room has a west exit.
        returnRoomExits ^= (ONE << ZERO);
    }

    return returnRoomExits;
}

static int loadRoom(void)
{
    initStruct();

    //*********** Get Title from file

    currentRoom.fieldLength = fgetc(fp);

    if (currentRoom.fieldLength == EOF)
    {
        return STANDARD_ERROR;
    }
    else
    {
        for (loop_ctr=ZERO; loop_ctr < currentRoom.fieldLength; loop_ctr++)
        {

            currentRoom.Title[loop_ctr] = fgetc(fp);

            if (currentRoom.Title[loop_ctr] == EOF)
            {
                currentRoom.Title[loop_ctr] = charNULL;
                return STANDARD_ERROR;
            }
        }
        // Add NULL at end
        currentRoom.Title[loop_ctr] = charNULL;
    }

    //*********** Get Item requirements from file and check if player has it in inventory
    while (1)
    {
        currentRoom.fieldLength = fgetc(fp);

        if (currentRoom.fieldLength != EOF)
        {
            item_found = TRUE;
            for (loop_ctr=ZERO; loop_ctr < currentRoom.fieldLength; loop_ctr++)
            {
                // cast fgetc since expecting binary data
                currentRoom.Item_Requirements[loop_ctr] = (uint8_t) fgetc(fp);

                if (currentRoom.Item_Requirements[loop_ctr] == EOF)
                {
                    currentRoom.Item_Requirements[loop_ctr] = charNULL;
                    return STANDARD_ERROR;
                }

                if (!FindInInventory(currentRoom.Item_Requirements[loop_ctr]))
                {
                    item_found = FALSE;
                }
            }
        }
        // If Items in player inventory does not much item requirement then
        // Go to next version of room (if available) as this is not the correct room

        // Bypass description, item contained, exits
        if (item_found == FALSE)
        {
            // Bypass Description
            tempLength = fgetc(fp);
            if (tempLength == EOF)
            {
                return STANDARD_ERROR;
            }
            else
            {
                for (loop_ctr=ZERO; loop_ctr < tempLength; loop_ctr++)
                {
                    if (fgetc(fp) == EOF)
                    {
                        return STANDARD_ERROR;
                    }
                }
            }

            //Bypass Item Contained
            tempLength = fgetc(fp);
            if (tempLength == EOF)
            {
                return STANDARD_ERROR;
            }
            else
            {
                for (loop_ctr=ZERO; loop_ctr < tempLength; loop_ctr++)
                {
                    if (fgetc(fp) == EOF)
                    {
                        return STANDARD_ERROR;
                    }
                }
            }

            //Bypass Exits
            for (loop_ctr=ZERO; loop_ctr < GAME_MAX_ROOM_EXITS; loop_ctr++)
            {
                if (fgetc(fp) == EOF)
                {
                    return STANDARD_ERROR;
                }
            }

            //Remove previous item requirements from struct
            for (loop_ctr=ZERO; loop_ctr < GAME_MAX_ROOM_ITEMS; loop_ctr++)
            {
                currentRoom.Item_Requirements[loop_ctr] = ZERO;
            }
        }
        else
        {
            // All items required is in player's inventory so okay to go out of while loop
            break;
        }
    }
    //End of While (1)

//*********** Get Room Description from file

    currentRoom.fieldLength = fgetc(fp);

    if (currentRoom.fieldLength == EOF)
    {
        return STANDARD_ERROR;
    }
    else
    {
        for (loop_ctr=ZERO; loop_ctr < currentRoom.fieldLength; loop_ctr++)
        {
            currentRoom.Desc[loop_ctr] = fgetc(fp);

            if (currentRoom.Desc[loop_ctr] == EOF)
            {
                currentRoom.Desc[loop_ctr] = charNULL;
                return STANDARD_ERROR;
            }
        }
        // Add NULL at end
        currentRoom.Desc[loop_ctr] = charNULL;
    }

//*********** Get Items contained from file and add to player's inventory

    currentRoom.fieldLength = fgetc(fp);

    if (currentRoom.fieldLength == EOF)
    {
        return STANDARD_ERROR;
    }
    else
    {
        for (loop_ctr=ZERO; loop_ctr < currentRoom.fieldLength; loop_ctr++)
        {
            // cast fgetc since expecting binary data
            currentRoom.Item_Contained[loop_ctr] = (uint8_t) fgetc(fp);

            // Add item found in room to player's inventory of items
            AddToInventory(currentRoom.Item_Contained[loop_ctr]);

            if (currentRoom.Item_Contained[loop_ctr] == EOF)
            {
                currentRoom.Item_Contained[loop_ctr] = charNULL;
                return STANDARD_ERROR;
            }
        }
    }

//*********** Get Exit rooms from file

    for (loop_ctr=ZERO; loop_ctr < GAME_MAX_ROOM_EXITS; loop_ctr++)
    {
         tempExit = (uint8_t) fgetc(fp);
         if (tempExit == charNULL )
         {
             currentRoom.Exits[loop_ctr] = ZERO;
         }else
         {
             currentRoom.Exits[loop_ctr] = tempExit;
         }

        if (currentRoom.Exits[loop_ctr] == EOF)
        {
            currentRoom.Exits[loop_ctr] = charNULL;
            return STANDARD_ERROR;
        }
    }
// Close the file
    fclose(fp);
    fp = NULL;

    return SUCCESS;
}

static void initStruct(void)
{

//Title
    for (loop_ctr=ZERO; loop_ctr < GAME_MAX_ROOM_TITLE_LENGTH; loop_ctr++)
    {
        currentRoom.Title[loop_ctr] = charNULL;
    };

//Item_Requirements
    for (loop_ctr=ZERO; loop_ctr < GAME_MAX_ROOM_ITEMS; loop_ctr++)
    {
        currentRoom.Item_Requirements[loop_ctr] = ZERO;
    };

//Desc
    for (loop_ctr=ZERO; loop_ctr < GAME_MAX_ROOM_DESC_LENGTH; loop_ctr++)
    {
        currentRoom.Desc[loop_ctr] = charNULL;
    }

//Item_Contained
    for (loop_ctr=ZERO; loop_ctr < GAME_MAX_ROOM_ITEMS; loop_ctr++)
    {
        currentRoom.Item_Contained[loop_ctr] = ZERO;
    }

//Exits
    for (loop_ctr=ZERO; loop_ctr < GAME_MAX_ROOM_EXITS; loop_ctr++)
    {
        currentRoom.Exits[loop_ctr] = ZERO;
    }
}

