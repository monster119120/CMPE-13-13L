/*
 * File: lab8.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Modified on February 28, 2016, 1:11 PM
 */

// HEAP size = 4096

// **** Include libraries here ****
// Standard C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Ascii.h"
//#include "Buttons.h" // will comment out after Unit Test
#include "Oled.h"
#include "OledDriver.h"
#include "Tree.h"
#include "Morse.h"

// **** Set any macros or preprocessor directives here ****
// Specify a bit mask for setting/clearing the pin corresponding to BTN4. Should only be used when
// unit testing the Morse event checker.
#define BUTTON4_STATE_FLAG (1 << 7)
#define SPACE ' '

// **** Declare any data types here ****

// **** Define any module-level, global, or external variables here ****
static uint8_t morseEvent = FALSE; // stores status of a button event
static char OledString[100]; // argument of OledDrawString()
static char topLine[10]; // displays Morse code
static char bottomLine[64]; // displays decoded letters
static uint8_t loop_ctr = 0;
static uint8_t topline_array_ptr = 0;
static uint8_t bottomline_array_ptr = 0;
static char letter;

// **** Declare any function prototypes here ****
/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
static void OledClearTopLine();
static void OledAppendTopLine(MorseChar dot_dash);
static void OledAppendBottomLine(char letter);
static void OledInitializeVars();

int main()
{
    BOARD_Init();


    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    // Invoke Unit test function
    // Unit_Test_MorseCheckEvent();

    // Initialize Oled screen
    OledInit();
    // This function populates spaces into topLine and bottomLine Arrays
    OledInitializeVars();

    // morseEvent is populated in 100Hz timer Interrupt call to MorseCheckEvents()
    if (!MorseInit()) {
        printf("Error, failed to initialize Morse decoder.\n");
        FATAL_ERROR();
    } else {
        // Test Oled display
        // Unit_Test_OledDisplay();

        while (1) {
            switch (morseEvent) {
            case MORSE_EVENT_INTER_WORD:
                if (topline_array_ptr > 5) {
                    OledClearTopLine();
                    OledAppendBottomLine(MORSE_CHAR_END_OF_CHAR);
                    MorseDecode(MORSE_CHAR_DECODE_RESET);
                } else {
                    letter = MorseDecode(MORSE_CHAR_END_OF_CHAR);
                    OledAppendBottomLine(letter);
                    OledClearTopLine();
                    MorseDecode(MORSE_CHAR_DECODE_RESET);
                    letter = SPACE;
                    OledAppendBottomLine(letter);
                }
                morseEvent = MORSE_EVENT_NONE;
                break;
            case MORSE_EVENT_INTER_LETTER:
                if (topline_array_ptr > 5) {
                    OledClearTopLine();
                    OledAppendBottomLine(MORSE_CHAR_END_OF_CHAR);
                    MorseDecode(MORSE_CHAR_DECODE_RESET);
                } else {
                    letter = MorseDecode(MORSE_CHAR_END_OF_CHAR);
                    OledAppendBottomLine(letter);
                    OledClearTopLine();
                    MorseDecode(MORSE_CHAR_DECODE_RESET);
                }
                morseEvent = MORSE_EVENT_NONE;
                break;
            case MORSE_EVENT_DOT:
                if (topline_array_ptr > 5) {
                    OledClearTopLine();
                    OledAppendBottomLine(MORSE_CHAR_END_OF_CHAR);
                    MorseDecode(MORSE_CHAR_DECODE_RESET);
                } else {
                    OledAppendTopLine(MORSE_CHAR_DOT);
                    MorseDecode(MORSE_CHAR_DOT);
                }
                morseEvent = MORSE_EVENT_NONE;
                break;
            case MORSE_EVENT_DASH:
                if (topline_array_ptr > 5) {
                    OledClearTopLine();
                    OledAppendBottomLine(MORSE_CHAR_END_OF_CHAR);
                    MorseDecode(MORSE_CHAR_DECODE_RESET);
                } else {
                    OledAppendTopLine(MORSE_CHAR_DASH);
                    MorseDecode(MORSE_CHAR_DASH);
                }
                morseEvent = MORSE_EVENT_NONE;
                break;
            }
        }
    }

    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks.
     *****************************************************************************/

    while (1);
}


// Functions that output to the Oled

static void OledClearTopLine()
{
    for (loop_ctr = 0; loop_ctr < 11; loop_ctr++) {
        topLine[loop_ctr] = SPACE;
    }
    sprintf(OledString, "%s\n%s", topLine, bottomLine);
    topline_array_ptr = 0;
    OledDrawString(OledString);
    OledUpdate();
    return;
}

static void OledAppendTopLine(MorseChar dot_dash)
{
    topLine[topline_array_ptr] = dot_dash;
    sprintf(OledString, "%s\n%s", topLine, bottomLine);
    OledDrawString(OledString);
    OledUpdate();
    topline_array_ptr++;
    return;
}

static void OledAppendBottomLine(char letter)
{
    bottomLine[bottomline_array_ptr] = letter;
    sprintf(OledString, "%s\n%s", topLine, bottomLine);
    OledDrawString(OledString);
    OledUpdate();
    if (bottomline_array_ptr < 63) {
        bottomline_array_ptr++;
    }
    return;
}

static void OledInitializeVars()
{
    for (loop_ctr = 0; loop_ctr < 11; loop_ctr++) {
        topLine[loop_ctr] = SPACE;
    }

    for (loop_ctr = 0; loop_ctr < 63; loop_ctr++) {
        bottomLine[loop_ctr] = SPACE;
    }
}

void __ISR(_TIMER_2_VECTOR, IPL4AUTO) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    // Call MorseCheckEvents() and store it in morseEvent
    morseEvent = MorseCheckEvents();
}
