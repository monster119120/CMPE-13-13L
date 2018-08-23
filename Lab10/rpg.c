/*
 * File: rpg.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Modified on March 13, 2016, 10:04 AM
 */

// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "UnixBOARD.h"
#include "Game.h"




// User libraries


// **** Set any macros or preprocessor directives here ****
#define ASCII_ESC 27
#define ZERO 0
#define ONE 1
#define FOUR 4
#define charNULL '\0'
#define NORTH 'n'
#define SOUTH 's'
#define EAST 'e'
#define WEST 'w'
#define SUCCESS_TERMINATE() exit(0)



// **** Declare any data types here ****

// **** Define any global or external variables here ****

static char direction;
static uint8_t TitleLength;
static uint8_t DescLength;
static uint8_t returnRoomExits;
static uint8_t life_ctr = FOUR;
static char RoomTitle[GAME_MAX_ROOM_TITLE_LENGTH];
static char RoomDesc[GAME_MAX_ROOM_DESC_LENGTH + ONE];

// **** Declare any function prototypes here ****



int main()
{
    /******************************** Your custom code goes below here ********************************/
    //Get Initial Room (32)
    if (!GameInit())
    {
        //If GameInit() fails
        FATAL_ERROR();
    }
    else
    {
        while (1)
        {
            if (life_ctr == ZERO)
            {
                // Exit of while loop if you tried nesw and didn't got stuck in room
                SUCCESS_TERMINATE();
            }

            returnRoomExits = GameGetCurrentRoomExits();

            // Reset color to default
            printf( "%c[0m", ASCII_ESC );
            // Clear the Screen - cursor moves to home
            printf( "%c[2J", ASCII_ESC );
            // Move cursor to top left corner
            printf( "%c[H", ASCII_ESC );
            //Room Title

            TitleLength = GameGetCurrentRoomTitle(RoomTitle);
            // Color Blue background with white text
            printf( "%c[44;1m", ASCII_ESC );
            printf(RoomTitle);

            //Room Description

            //Move cursor to next line
            printf( "%c[E", ASCII_ESC );
            // Reset color to default
            printf( "%c[0m", ASCII_ESC );

            // Color Light Green with black background
            printf( "%c[33m", ASCII_ESC );
            DescLength = GameGetCurrentRoomDescription(RoomDesc);
            printf(RoomDesc);

            // Move cursor 18 lines down
            printf( "%c[18B", ASCII_ESC );
            //Move cursor to beginning of next line
            printf( "%c[E", ASCII_ESC );
            // Display NESW


            // Move cursor right
            printf( "%c[10C", ASCII_ESC );


            if ((returnRoomExits) & GAME_ROOM_EXIT_NORTH_EXISTS) {
                // Green
                printf( "%c[37;42m", ASCII_ESC );
            } else {
                // Red
                printf( "%c[37;41m", ASCII_ESC );
            }
            printf("North");

            // Move cursor 1 line down
            printf( "%c[1B", ASCII_ESC );
            //Move cursor to beginning of next line
            printf( "%c[E", ASCII_ESC );
            // Move cursor right
            printf( "%c[3C", ASCII_ESC );
            //printf( "%c[10C", ASCII_ESC );

            if ((returnRoomExits) & GAME_ROOM_EXIT_EAST_EXISTS) {
                // Green
                printf( "%c[37;42m", ASCII_ESC );
            } else {
                // Red
                printf( "%c[37;41m", ASCII_ESC );
            }
            printf("East");

            // Move cursor right
            printf( "%c[11C", ASCII_ESC );
            //printf( "%c[10C", ASCII_ESC );

            if ((returnRoomExits) & GAME_ROOM_EXIT_WEST_EXISTS) {
                printf( "%c[37;42m", ASCII_ESC );
            } else {
                printf( "%c[37;41m", ASCII_ESC );
            }
            printf("West");


            // Move cursor 1 line down
            printf( "%c[1B", ASCII_ESC );
            //Move cursor to beginning of next line
            printf( "%c[E", ASCII_ESC );
            // Move cursor right
            printf( "%c[10C", ASCII_ESC );

            if ((returnRoomExits) & GAME_ROOM_EXIT_SOUTH_EXISTS) {
                 // Green
                printf( "%c[37;42m", ASCII_ESC );
            }else {
                // Red
                printf( "%c[37;41m", ASCII_ESC );
            }
            printf("South");


            //Move cursor to beginning of next line
            printf( "%c[E", ASCII_ESC );

            // Reset color to default
            printf( "%c[0m", ASCII_ESC );
            // White with black background
            printf( "%c[97m", ASCII_ESC );
            printf("Enter Direction to travel (n,e,s,w) followed by enter: ");

            // Keyboard Input
            direction = getchar();

            if (direction == NORTH)
            {
                if ((returnRoomExits) & GAME_ROOM_EXIT_NORTH_EXISTS)
                {
                    GameGoNorth();
                }
            }

            if (direction == EAST)
            {
                if ((returnRoomExits) & GAME_ROOM_EXIT_EAST_EXISTS)
                {
                    GameGoEast();
                }
            }

            if (direction == SOUTH)
            {
                if ((returnRoomExits) & GAME_ROOM_EXIT_SOUTH_EXISTS)
                {

                    GameGoSouth();
                }
            }

            if (direction == WEST)
            {
                if ((returnRoomExits) & GAME_ROOM_EXIT_WEST_EXISTS)
                {
                    GameGoWest();
                }
            }
            // If no exit available then you die
            if (!returnRoomExits)
            {
                life_ctr = ZERO;
            }

        }
    }
}

/**************************************************************************************************/





