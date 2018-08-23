/*
 * File: bounce_part3.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Modified on February 17, 2016, 12:39 PM
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>

// CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Leds.h"
#include "Buttons.h"

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

// **** Define global, module-level, or external variables here ****
uint8_t buttonStates = 0;
unsigned short LED_display = 0;

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
    // Initialize LEDs for output
    LEDS_INIT();

    // Initialize buttons
    ButtonsInit();

    while (1) {

        //        Unit Test Code for Part3
        //        
        //        // Set LED to 0
        //        LED_display = BUTTON_EVENT_NONE;
        //        
        //        if (buttonStates & BUTTON_EVENT_1UP) {
        //            puts("BUTTON_EVENT_1UP");
        //        } else if (buttonStates & BUTTON_EVENT_1DOWN) {
        //            puts("BUTTON_EVENT_1DOWN");
        //            LED_display |= 0x3;
        //        }
        //
        //        if (buttonStates & BUTTON_EVENT_2UP) {
        //            puts("BUTTON_EVENT_2UP");
        //        } else if (buttonStates & BUTTON_EVENT_2DOWN) {
        //            puts("BUTTON_EVENT_2DOWN");
        //            LED_display |= 0xC;
        //        }
        //
        //        if (buttonStates & BUTTON_EVENT_3UP) {
        //            puts("BUTTON_EVENT_3UP");
        //        } else if (buttonStates & BUTTON_EVENT_3DOWN) {
        //            puts("BUTTON_EVENT_3DOWN");
        //            LED_display |= 0x30;
        //        }
        //
        //        if (buttonStates & BUTTON_EVENT_4UP) {
        //            puts("BUTTON_EVENT_4UP");
        //        } else if (buttonStates & BUTTON_EVENT_4DOWN) {
        //            puts("BUTTON_EVENT_4DOWN");
        //            LED_display |= 0xC0;
        //        }

        if (buttonStates) {
            // Set LED to 0
            LED_display = BUTTON_EVENT_NONE;

            // Switch 1 and Button 1
            if (SWITCH_STATES() & SWITCH_STATE_SW1) { // switch 1 is up 
                if (buttonStates & BUTTON_EVENT_1UP) { // button 1 is up
                    // Set LEDs 1 and 2 0000 0011
                    LED_display |= 0x3;
                }
            } else { // switch 1 is down
                if ((buttonStates & BUTTON_EVENT_1DOWN)) { // button 1 is down
                    // Set LEDs 1 and 2 0000 0011
                    LED_display |= 0x3;
                }
            }

            // Switch 2 and Button 2
            if (SWITCH_STATES() & SWITCH_STATE_SW2) { // switch 2 is up
                if (buttonStates & BUTTON_EVENT_2UP) { // button 2 is up
                    // Set LEDs 3 and 4 0000 1100
                    LED_display |= 0xc;
                }
            } else { // switch 2 is down
                if (buttonStates & BUTTON_EVENT_2DOWN) { // button 2 is down
                    // Set LEDs 3 and 4 0000 1100
                    LED_display |= 0xc;
                }
            }

            // Switch 3 and Button 3           
            if (SWITCH_STATES() & SWITCH_STATE_SW3) { // switch 3 is up
                if (buttonStates & BUTTON_EVENT_3UP) { // button 3 is up
                    // Set LEDs 5 and 6 0011 0000 
                    LED_display |= 0x30;
                }
            } else { // switch 3 is down
                if (buttonStates & BUTTON_EVENT_3DOWN) { // button 3 is down
                    // Set LEDs 5 and 6 0011 0000 
                    LED_display |= 0x30;
                }
            }

            // Switch 4 and Button 4
            if (SWITCH_STATES() & SWITCH_STATE_SW4) { // switch 4 is up
                if (buttonStates & BUTTON_EVENT_4UP) { // button 4 is up
                    // Set LEDs 7 and 8 1100 0000 
                    LED_display |= 0xc0;
                }
            } else { // switch 4 is down
                if (buttonStates & BUTTON_EVENT_4DOWN) { // button 4 is down
                    // Set LEDs 7 and 8 1100 0000 
                    LED_display |= 0xc0;
                }
            }

            // Display LEDs based on status of buttons/switches
            LEDS_SET(LED_display);
        }
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);

}

/**
 * This is the interrupt for the Timer1 peripheral. It checks for button events and stores them in a
 * module-level variable.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{
    // Clear the interrupt flag.
    INTClearFlag(INT_T1);

    // Call button event function in Buttons.c
    buttonStates = ButtonsCheckEvents();
}
