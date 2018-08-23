/*
 * File: bounce_part1.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Modified on February 15, 2016, 4:28 PM
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Leds.h"

// **** Set macros and preprocessor directives ****
#define _SUPPRESS_PLIB_WARNING

// Direction of LEDS
#define LEFT 1
#define RIGHT 0

// **** Declare any datatypes here ****

struct TimerResult {
    uint8_t event;
    unsigned char value;
};

// **** Define global, module-level, or external variables here ****
struct TimerResult myTimerResult;
int delay = 1000000;
int switch_value = 0;
uint8_t switchesState;
int mask = 1;
int i;

// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, 0xFFFF);

    // Set up the timer interrupt with a priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    /* Unit Test Part1
    LEDS_INIT();
    int i;
    int late_val;
    LEDS_SET(0xFF);
    for (i = 0; i < 10000000; ++i);
    late_val = LEDS_GET();
    if (late_val == 0xFF) {
       printf(" Match: %d\n",late_val);
       // LEDS_SET(0x18);
    }
    LEDS_SET(0xF0);
    for (i = 0; i < 10000000; ++i); 
    LEDS_SET(0xF); 
    for (i = 0; i < 10000000; ++i); 
    LEDS_SET(0); */

    // Initialize LEDs with left bounce direction
    LEDS_INIT();
    int direction = LEFT;

    while (1) {
        LEDS_SET(mask);

        if (myTimerResult.event == TRUE) {
            delay = 1000000; // delay to initial value

            if (switchesState & SWITCH_STATE_SW1) {
                // Switch 1 is on.
                delay = delay + 1000000;
            }
            if (switchesState & SWITCH_STATE_SW2) {
                // Switch 2 is on.
                delay = delay + 1500000;
            }
            if (switchesState & SWITCH_STATE_SW3) {
                // Switch 3 is on.
                delay = delay + 2000000;
            }
            if (switchesState & SWITCH_STATE_SW4) {
                // Switch 4 is on.
                delay = delay + 3000000;
            }
        }

        for (i = 0; i < delay; ++i);
        int late_val = LEDS_GET();

        if (late_val == 128) {
            // LED 7 is lit then go right
            direction = RIGHT;
        }
        if (late_val == 1) {
            // LED 1 is lit then go left
            direction = LEFT;
        }

        if (direction == LEFT) {
            mask = mask << 1;
        } else {
            mask = mask >> 1;
        }
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. During each call it increments a counter (the
 * value member of a module-level TimerResult struct). This counter is then checked against the
 * binary values of the four switches on the I/O Shield (where SW1 has a value of 1, SW2 has a value
 * of 2, etc.). If the current counter is greater than this switch value, then the event member of a
 * module-level TimerResult struct is set to true and the value member is cleared.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{
    switchesState = SWITCH_STATES();
    switch_value = 0;

    if (switchesState & SWITCH_STATE_SW1) {
        // Switch 1 is on.
        switch_value = switch_value + 1;
    }
    if (switchesState & SWITCH_STATE_SW2) {
        // Switch 2 is on.
        switch_value = switch_value + 2;
    }
    if (switchesState & SWITCH_STATE_SW3) {
        // Switch 3 is on.
        switch_value = switch_value + 4;
    }
    if (switchesState & SWITCH_STATE_SW4) {
        // Switch 4 is on.
        switch_value = switch_value + 8;
    }

    if (myTimerResult.value >= switch_value) {
        myTimerResult.event = TRUE;
        myTimerResult.value = 0;
    } else {
        myTimerResult.event = FALSE;
        myTimerResult.value++;
    }

    // Clear the interrupt flag.
    INTClearFlag(INT_T1);
}
