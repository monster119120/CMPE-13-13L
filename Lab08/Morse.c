/*
 * File: Morse.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Modified on February 28, 2016, 1:14 PM
 */

// **** Include libraries here ****
// Standard C libraries
#include <stdio.h>
#include <stdlib.h>

// User libraries
#include "Buttons.h"
#include "Morse.h"
#include "Tree.h"

// **** Define any module-level, global, or external variables here ****
static uint8_t buttonEvent = FALSE; // stores status of a button event
static uint8_t Morse_event_value; // stores returned morse event value 
static uint16_t MorseCheckEvent_tick_counter = 0; // stores ticks during a button event of Button 4
static uint8_t FSM_STATE; // stores value of current state
static uint8_t first_time = TRUE; // first time flag 

// Char String of six level complete Morse Binary tree
char morseString[] = {0, 'E', 'I', 'S', 'H', '5', '4', 'V', 0, '3', 'U', 'F', 0,
    0, 0, 0, '2', 'A', 'R', 'L', 0, 0, 0, 0, 0, 'W', 'P', 0, 0, 'J',
    0, '1', 'T', 'N', 'D', 'B', '6', 0, 'X', 0, 0, 'K', 'C', 0, 0, 'Y', 0,
    0, 'M', 'G', 'Z', '7', 0, 'Q', 0, 0, 'O', 0, '8', 0, 0, '9', '0'};

// Node variables
Node *traversedString; // used in loading tree and in tree traversal
Node *savedString; // stores top most node    

// Enum for FSM

enum {
    WAITING_STATE,
    DOT_STATE,
    DASH_STATE,
    INTERLETTER_STATE,
} MORSE_STATE = WAITING_STATE;

/**
 * This function initializes the Morse code decoder. This is primarily the generation of the
 * Morse tree: a binary tree consisting of all of the ASCII alphanumeric characters arranged
 * according to the DOTs and DASHes that represent each character. Traversal of the tree is done by
 * taking the left-child if it is a dot and the right-child if it is a dash. If the morse tree is
 * successfully generated, SUCCESS is returned, otherwise STANDARD_ERROR is returned. This function
 * also initializes the Buttons library so that MorseCheckEvents() can work properly.
 * @return Either SUCCESS if the decoding tree was successfully created or STANDARD_ERROR if not.
 */
int MorseInit(void)
{
    // Initialize buttons here
    ButtonsInit();

    // Initialize Morse binary tree
    traversedString = TreeCreate(6, morseString);
    // Saving pointer of traversedString for use later
    savedString = traversedString;

    if (traversedString) {
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/**
 * MorseDecode decodes a Morse string by iteratively being passed MORSE_CHAR_DOT or MORSE_CHAR_DASH.
 * Each call that passes a DOT or DASH returns a SUCCESS if the string could still compose a
 * Morse-encoded character. Passing in the special MORSE_CHAR_END_OF_CHAR constant will terminate
 * decoding and return the decoded character. During that call to MorseDecode() the return value
 * will be the character that was decoded or STANDARD_ERROR if it couldn't be decoded. Another
 * special value exists, MORSE_CHAR_DECODE_RESET, which will clear the stored state. When a
 * MORSE_CHAR_DECODE_RESET is done, SUCCESS will be returned. If the input is not a valid MorseChar
 * then the internal state should be reset and STANDARD_ERROR should be returned.
 * 
 * @param in A value from the MorseChar enum which specifies how to traverse the Morse tree.
 * 
 * @return Either SUCCESS on DECODE_RESET or when the next traversal location is still a valid
 *         character, the decoded character on END_OF_CHAR, or STANDARD_ERROR if the Morse tree
 *         hasn't been initialized, the next traversal location doesn't exist/represent a character,
 *         or `in` isn't a valid member of the MorseChar enum.
 */
char MorseDecode(MorseChar in)
{
    if (in == MORSE_CHAR_DOT) {
        traversedString = traversedString->leftChild;
        return SUCCESS;
    } else if (in == MORSE_CHAR_DASH) {
        traversedString = traversedString->rightChild;
        return SUCCESS;
    } else if (in == MORSE_CHAR_END_OF_CHAR) {
        if (traversedString->data == '\0') {
            return MORSE_CHAR_END_OF_CHAR;
        } else {
            return traversedString->data;
        }
    } else if (in == MORSE_CHAR_DECODE_RESET) {
        // reset traversedString to saved head node to point to top of binary tree
        traversedString = savedString;
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/**
 * This function calls ButtonsCheckEvents() once per call and returns which, if any,
 * of the Morse code events listed in the enum above have been encountered. It checks for BTN4
 * events in its input and should be called at 100Hz so that the timing works. The
 * length that BTN4 needs to be held down for a dot is >= 0.25s and < 0.50s with a dash being a button
 * down event for >= 0.5s. The button uptime various between dots/dashes (>= .5s), letters
 * (>= 1s), and words (>= 2s).
 *
 * @note This function assumes that the buttons are all unpressed at startup, so that the first
 *       event it will see is a BUTTON_EVENT_*DOWN.
 *
 * So pressing the button for 0.1s, releasing it for 0.1s, pressing it for 0.3s, and then waiting
 * will decode the string '.-' (A). It will trigger the following order of events:
 * 9 MORSE_EVENT_NONEs, 1 MORSE_EVENT_DOT, 39 MORSE_EVENT_NONEs, a MORSE_EVENT_DASH, 69
 * MORSE_EVENT_NONEs, a MORSE_EVENT_END_CHAR, and then MORSE_EVENT_INTER_WORDs.
 * 
 * @return The MorseEvent that occurred.
 */
MorseEvent MorseCheckEvents(void)
{
    // Get the buttonEvent by calling ButtonsCheckEvents()
    buttonEvent = ButtonsCheckEvents();

    // Increment every time MorseCheckEvents is invoked
    MorseCheckEvent_tick_counter++;

    // If first time then ignore value in FSM_STATE
    if (first_time == TRUE) {
        first_time = FALSE;
    } else {
        MORSE_STATE = FSM_STATE;
    }

    // Default return Morse Event Value to MORSE_EVENT_NONE
    Morse_event_value = MORSE_EVENT_NONE;

    // Morse Event FSM
    switch (MORSE_STATE) {
    case WAITING_STATE:
        if (buttonEvent & BUTTON_EVENT_4DOWN) {
            MorseCheckEvent_tick_counter = 0;
            FSM_STATE = DOT_STATE;
        } else {
            FSM_STATE = WAITING_STATE;
        }
        break;
    case DOT_STATE:
        if (buttonEvent & BUTTON_EVENT_4UP) {
            MorseCheckEvent_tick_counter = 0;
            Morse_event_value = MORSE_EVENT_DOT;
            FSM_STATE = INTERLETTER_STATE;
        } else {
            if (MorseCheckEvent_tick_counter >= MORSE_EVENT_LENGTH_DOWN_DOT) {
                FSM_STATE = DASH_STATE;
            }
        }
        break;
    case DASH_STATE:
        if (buttonEvent & BUTTON_EVENT_4UP) {
            MorseCheckEvent_tick_counter = 0;
            Morse_event_value = MORSE_EVENT_DASH;
            FSM_STATE = INTERLETTER_STATE;
        }
        break;
    case INTERLETTER_STATE:
        if (buttonEvent & BUTTON_EVENT_4DOWN) {
            if (MorseCheckEvent_tick_counter >= MORSE_EVENT_LENGTH_UP_INTER_LETTER) {
                MorseCheckEvent_tick_counter = 0;
                FSM_STATE = DOT_STATE;
                Morse_event_value = MORSE_EVENT_INTER_LETTER;
            } else {
                MorseCheckEvent_tick_counter = 0;
                FSM_STATE = DOT_STATE;
                Morse_event_value = MORSE_EVENT_NONE;
            }

        } else {
            if (MorseCheckEvent_tick_counter >= MORSE_EVENT_LENGTH_UP_INTER_WORD) {
                Morse_event_value = MORSE_EVENT_INTER_WORD;
                FSM_STATE = WAITING_STATE;
            }
        }
        break;
    }
    return Morse_event_value;
}
