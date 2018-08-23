/*
 * File: Field.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Modified on March 7, 2016, 10:02 PM
 */

// Standard C libraries
#include <stdio.h>
#include <stdlib.h>

// User libraries
#include "Field.h"
#include "Protocol.h"

// CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
#define ZERO 0
#define ONE 1
#define FIVE 5
#define NINE 9

static uint8_t row_ctr;
static uint8_t col_ctr;
static uint8_t boatSize;
static uint8_t FieldPosition_value;

/**
 * FieldInit() will fill the passed field array with the data specified in positionData. Also the
 * lives for each boat are filled according to the `BoatLives` enum.
 * @param f The field to initialize.
 * @param p The data to initialize the entire field to, should be a member of enum
 *                     FieldPosition.
 */
void FieldInit(Field *f, FieldPosition p)
{
    for (row_ctr = 0; row_ctr < FIELD_ROWS; row_ctr++) { // outer for loop for row
        for (col_ctr = 0; col_ctr < FIELD_COLS; col_ctr++) { // inter for loop for col
            f->field[row_ctr][col_ctr] = p; // set value in field position
        }
    }

    // Set initial both lives for all boat types from BoatLive enum
    f->smallBoatLives = FIELD_BOAT_LIVES_SMALL;
    f->mediumBoatLives = FIELD_BOAT_LIVES_MEDIUM;
    f->largeBoatLives = FIELD_BOAT_LIVES_LARGE;
    f->hugeBoatLives = FIELD_BOAT_LIVES_HUGE;

    return;
}

/**
 * Retrieves the value at the specified field position.
 * @param f The Field being referenced
 * @param row The row-component of the location to retrieve
 * @param col The column-component of the location to retrieve
 * @return
 */
FieldPosition FieldAt(const Field *f, uint8_t row, uint8_t col)
{
    return f->field[row][col];
}

/**
 * This function provides an interface for setting individual locations within a Field struct. This
 * is useful when FieldAddBoat() doesn't do exactly what you need. For example, if you'd like to use
 * FIELD_POSITION_CURSOR, this is the function to use.
 * 
 * @param f The Field to modify.
 * @param row The row-component of the location to modify
 * @param col The column-component of the location to modify
 * @param p The new value of the field location
 * @return The old value at that field location
 */
FieldPosition FieldSetLocation(Field *f, uint8_t row, uint8_t col, FieldPosition p)
{
    // Define oldPosition and save value
    FieldPosition oldPosition = FieldAt(f, row, col);

    f->field[row][col] = p; // Update the value in field[row][col] with p
    return oldPosition; // Return old position value
}

/**
 * FieldAddBoat() places a single ship on the player's field based on arguments 2-5. Arguments 2, 3
 * represent the x, y coordinates of the pivot point of the ship.  Argument 4 represents the
 * direction of the ship, and argument 5 is the length of the ship being placed. All spaces that
 * the boat would occupy are checked to be clear before the field is modified so that if the boat
 * can fit in the desired position, the field is modified as SUCCESS is returned. Otherwise the
 * field is unmodified and STANDARD_ERROR is returned. There is no hard-coded limit to how many
 * times a boat can be added to a field within this function.
 *
 * So this is valid test code:
 * {
 *   Field myField;
 *   FieldInit(&myField,FIELD_POSITION_EMPTY);
 *   FieldAddBoat(&myField, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_SMALL);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_MEDIUM);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_HUGE);
 *   FieldAddBoat(&myField, 0, 6, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_SMALL);
 * }
 *
 * should result in a field like:
 *  _ _ _ _ _ _ _ _
 * [ 3 3 3       3 ]
 * [ 4 4 4 4     3 ]
 * [             3 ]
 *  . . . . . . . .
 *
 * @param f The field to grab data from.
 * @param row The row that the boat will start from, valid range is from 0 and to FIELD_ROWS - 1.
 * @param col The column that the boat will start from, valid range is from 0 and to FIELD_COLS - 1.
 * @param dir The direction that the boat will face once places, from the BoatDirection enum.
 * @param boatType The type of boat to place. Relies on the FIELD_POSITION_*_BOAT values from the
 * FieldPosition enum.
 * @return TRUE for success, FALSE for failure
 */
uint8_t FieldAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType type)
{
    // Initialize variables to ZERO
    row_ctr = ZERO;
    col_ctr = ZERO;
    boatSize = ZERO;
    FieldPosition_value = ZERO;

    // The type of boat to place relies on BoatType enum

    switch (type) {
    case FIELD_BOAT_SMALL:
        // BoatLives enum value
        // FIELD_BOAT_LIVES_SMALL = 3
        boatSize = FIELD_BOAT_LIVES_SMALL;
        FieldPosition_value = FIELD_POSITION_SMALL_BOAT;
        break;
    case FIELD_BOAT_MEDIUM:
        // BoatLives enum value
        // FIELD_BOAT_LIVES_MEDIUM = 4
        boatSize = FIELD_BOAT_LIVES_MEDIUM;
        FieldPosition_value = FIELD_POSITION_MEDIUM_BOAT;
        break;
    case FIELD_BOAT_LARGE:
        // BoatLives enum value
        // FIELD_BOAT_LIVES_LARGE = 5
        boatSize = FIELD_BOAT_LIVES_LARGE;
        FieldPosition_value = FIELD_POSITION_LARGE_BOAT;
        break;
    case FIELD_BOAT_HUGE:
        // BoatLives enum value
        // FIELD_BOAT_LIVES_HUGE = 6
        boatSize = FIELD_BOAT_LIVES_HUGE;
        FieldPosition_value = FIELD_POSITION_HUGE_BOAT;
        break;
    }

    // Validate direction - either NORTH, EAST, SOUTH, WEST
    if ((dir != FIELD_BOAT_DIRECTION_NORTH) && (dir != FIELD_BOAT_DIRECTION_EAST)
            && (dir != FIELD_BOAT_DIRECTION_SOUTH) && (dir != FIELD_BOAT_DIRECTION_WEST)) {
        return STANDARD_ERROR;
    } else {
        // row, col denotes front of boat
        switch (dir) {
        case FIELD_BOAT_DIRECTION_NORTH:
            // Check if boatSize goes beyond top border, return Standard Error
            if ((row - (boatSize - ONE)) < ZERO) {
                return STANDARD_ERROR;
            }

            for (row_ctr = row; row_ctr > (row - boatSize); row_ctr--) {
                // Check if field row, col is not empty
                if ((FieldAt(f, row_ctr, col)) != FIELD_POSITION_EMPTY) {
                    return STANDARD_ERROR;
                }
            }

            // If it did not return STANDARD ERROR then boat can be added to field
            for (row_ctr = row; row_ctr > (row - boatSize); row_ctr--) {
                // Set field row, col with boat type
                FieldSetLocation(f, row_ctr, col, FieldPosition_value);
            }
            return SUCCESS;
            break;

        case FIELD_BOAT_DIRECTION_EAST:
            // Check if boatSize goes beyond right border, return Standard Error
            if ((col + (boatSize - ONE)) > NINE) {
                return STANDARD_ERROR;
            }

            for (col_ctr = col; col_ctr < (col + boatSize); col_ctr++) {
                // Check if field row, col is not empty
                if ((FieldAt(f, row, col_ctr)) != FIELD_POSITION_EMPTY) {
                    return STANDARD_ERROR;
                }
            }

            // If it did not return STANDARD ERROR then boat can be added to field
            for (col_ctr = col; col_ctr < (col + boatSize); col_ctr++) {
                // Set field row, col with boat type
                FieldSetLocation(f, row, col_ctr, FieldPosition_value);
            }
            return SUCCESS;
            break;

        case FIELD_BOAT_DIRECTION_SOUTH:
            // Check if boatSize goes beyond column bottom border, return Standard Error
            if ((row + (boatSize - ONE)) > FIVE) {
                return STANDARD_ERROR;
            }

            for (row_ctr = row; row_ctr < (row + boatSize); row_ctr++) {
                // Check if field row,col is not empty
                if ((FieldAt(f, row_ctr, col)) != FIELD_POSITION_EMPTY) {
                    return STANDARD_ERROR;
                }
            }

            // If it did not return STANDARD ERROR then boat can be added to field
            for (row_ctr = row; row_ctr < (row + boatSize); row_ctr++) {
                // Set field row,col with boat type
                FieldSetLocation(f, row_ctr, col, FieldPosition_value);
            }
            return SUCCESS;
            break;

        case FIELD_BOAT_DIRECTION_WEST:
            // Check if boatSize goes beyond left border, return Standard Error
            if ((col - (boatSize - ONE)) < ZERO) {
                return STANDARD_ERROR;
            }

            for (col_ctr = col; col_ctr > (col - boatSize); col_ctr--) {
                // Check if field row, col is not empty
                if ((FieldAt(f, row, col_ctr)) != FIELD_POSITION_EMPTY) {
                    return STANDARD_ERROR;
                }
            }

            // If it did not return STANDARD ERROR then boat can be added to field
            for (col_ctr = col; col_ctr > (col - boatSize); col_ctr--) {
                // Set field row,col with boat type
                FieldSetLocation(f, row, col_ctr, FieldPosition_value);
            }
            return SUCCESS;
            break;
        }
    }
    return ZERO;
}

/**
 * This function registers an attack at the gData coordinates on the provided field. This means that
 * 'f' is updated with a FIELD_POSITION_HIT or FIELD_POSITION_MISS depending on what was at the
 * coordinates indicated in 'gData'. 'gData' is also updated with the proper HitStatus value
 * depending on what happened AND the value of that field position BEFORE it was attacked. Finally
 * this function also reduces the lives for any boat that was hit from this attack.
 * @param f The field to check against and update.
 * @param gData The coordinates that were guessed. The HIT result is stored in gData->hit as an
 *               output.
 * @return The data that was stored at the field position indicated by gData before this attack.
 */
FieldPosition FieldRegisterEnemyAttack(Field *f, GuessData *gData)
{
    // Using FieldAt()
    FieldPosition GuessPosition = FieldAt(f, gData->row, gData->col);

    // gData -> hit is updated from HitStatus enum from Protocol.h
    // FieldPosition is updated from FieldPosition enum in Field.h

    // Check if there is no boat in Guess Position
    if (GuessPosition == 0) {
        // 'f' is updated with a FIELD_POSITION_MISS 
        // 'gData' is updated with HIT_MISS 
        gData->hit = HIT_MISS;
        // f is updated to Miss 
        FieldSetLocation(f, gData->row, gData->col, FIELD_POSITION_MISS);
        // Return the value of that field position BEFORE it was attacked
        return GuessPosition;
    } else if ((GuessPosition > FIELD_POSITION_EMPTY) && (GuessPosition < FIELD_POSITION_MISS)) {
        // If the Guessed Position has a small, medium, large, or huge boat
        // 'f' is updated with a FIELD_POSITION_HIT 
        FieldSetLocation(f, gData->row, gData->col, FIELD_POSITION_HIT);
        // 'gData' is also updated with the HIT value 
        gData->hit = HIT_HIT;
        // Reduces the lives for any boat that was hit from this attack
        switch (GuessPosition) {
        case FIELD_POSITION_SMALL_BOAT:
            // Reduce the count of small boats left
            f->smallBoatLives--;
            break;
        case FIELD_POSITION_MEDIUM_BOAT:
            // Reduce the count of medium boats left
            f->mediumBoatLives--;
            break;
        case FIELD_POSITION_LARGE_BOAT:
            // Reduce the count of large boats left
            f->largeBoatLives--;
            break;
        case FIELD_POSITION_HUGE_BOAT:
            // Reduce the count of huge boats left
            f->hugeBoatLives--;
            break;
        default:
            break;
        }
        // Return the value of that field position BEFORE it was attacked
        return GuessPosition;
    } else {
        // Return the value of that field position BEFORE it was attacked
        return GuessPosition;
    }
}

/**
 * This function updates the FieldState representing the opponent's game board with whether the
 * guess indicated within gData was a hit or not. If it was a hit, then the field is updated with a
 * FIELD_POSITION_HIT at that position. If it was a miss, display a FIELD_POSITION_EMPTY instead, as
 * it is now known that there was no boat there. The FieldState struct also contains data on how
 * many lives each ship has. Each hit only reports if it was a hit on any boat or if a specific boat
 * was sunk, this function also clears a boats lives if it detects that the hit was a
 * HIT_SUNK_*_BOAT.
 * @param f The field to grab data from.
 * @param gData The coordinates that were guessed along with their HitStatus.
 * @return The previous value of that coordinate position in the field before the hit/miss was
 * registered.
 */
FieldPosition FieldUpdateKnowledge(Field *f, const GuessData *gData)
{
    // Using FieldAt() get FieldPosition to be returned by function
    FieldPosition Opponent_GuessPosition = FieldAt(f, gData->row, gData->col);

    // gData-> hit is updated from HitStatus enum from Protocol.h
    // FieldPosition is updated from FieldPosition enum in Field.h

    // Check the value of gData->hit 
    switch (gData->hit) {
    case HIT_MISS:
        // If it was a miss, display a FIELD_POSITION_EMPTY instead
        FieldSetLocation(f, gData->row, gData->col, FIELD_POSITION_EMPTY);
        break;
    case HIT_HIT:
        // If it was a hit, then the field is updated with a FIELD_POSITION_HIT at that position
        FieldSetLocation(f, gData->row, gData->col, FIELD_POSITION_HIT);
        break;
    case HIT_SUNK_SMALL_BOAT:
        // Clears a boats lives if it detects that the hit was a HIT_SUNK_*_BOAT.
        // Set count to zero meaning no small boats left
        f->smallBoatLives = ZERO;
        break;
    case HIT_SUNK_MEDIUM_BOAT:
        // Clears a boats lives if it detects that the hit was a HIT_SUNK_*_BOAT.
        // Set count to zero meaning no medium boats left
        f->mediumBoatLives = ZERO;
        break;
    case HIT_SUNK_LARGE_BOAT:
        // Clears a boats lives if it detects that the hit was a HIT_SUNK_*_BOAT.
        // Set count to zero meaning no large boats left
        f->largeBoatLives = ZERO;
        break;
    case HIT_SUNK_HUGE_BOAT:
        // Clears a boats lives if it detects that the hit was a HIT_SUNK_*_BOAT.
        // Set count to zero meaning no huge boats left
        f->hugeBoatLives = ZERO;
        break;
    }
    return Opponent_GuessPosition;
}

/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field *f)
{
    // Define return state and default as 'sunk' when set to ZERO
    uint8_t returnAllStates = ZERO;

    // Check the boat lives in f
    if (f->smallBoatLives != 0) {
        // Small Boat is still alive
        // Toggle Bit 0 to 1
        // FIELD_BOAT_STATUS_SMALL = 0x01
        returnAllStates ^= (1 << 0);
    }

    if (f->mediumBoatLives != 0) {
        // Medium Boat is still alive
        // Toggle Bit 1 to 1
        // FIELD_BOAT_STATUS_MEDIUM = 0x02
        returnAllStates ^= (1 << 1);
    }

    if (f->largeBoatLives != 0) {
        // Large Boat is still alive
        // Toggle Bit 2 to 1
        // FIELD_BOAT_STATUS_LARGE  = 0x04
        returnAllStates ^= (1 << 2);
    }

    if (f->hugeBoatLives != 0) {
        // Huge Boat is still alive
        // Toggle Bit 3 to 1
        // FIELD_BOAT_STATUS_HUGE   = 0x08
        returnAllStates ^= (1 << 3);
    }

    // Returns the states of all 4 boats as a 4-bit bitfield    
    return returnAllStates;
}
