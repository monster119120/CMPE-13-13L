/*
 * File: Buttons.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Created on February 17, 2016, 12:49 PM
 */

/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/*******************************************************************************
  This source file has NOT been generated by the MHC
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************
// Standard libraries
#include <stdio.h>

// CMPE13 Support Library
#include "BOARD.h"
#include "Buttons.h"

// Microchip libraries
#include <xc.h>


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
uint8_t new_buttonState;
uint8_t ret_buttonState;
static uint8_t buttonStatearray[4];
static int ctr = 1;


// *****************************************************************************
// *****************************************************************************
// Section: User Functions
// *****************************************************************************
// *****************************************************************************

/**
 * This function initializes the proper pins such that the buttons 1-4 may be used by modifying
 * the necessary bits in TRISD/TRISF. Only the bits necessary to enable the 1-4 buttons are
 * modified so that this library does not interfere with other libraries.
 */
void ButtonsInit(void)
{
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
    return;
}

/**
 * This function checks the button states and returns any events that have occurred since the last
 * call. In the case of the first call to ButtonsCheckEvents() after ButtonsInit(), the function
 * should assume that the buttons start in an off state with value 0. Therefore if no buttons are
 * pressed when ButtonsCheckEvents() is first called, BUTTONS_EVENT_NONE should be returned. The
 * events are listed in the ButtonEventFlags enum at the top of this file. This function should be
 * called repeatedly.
 *
 * This function also performs debouncing of the buttons. Every time ButtonsCheckEvents() is called,
 * all button values are saved, up to the 4th sample in the past, so 4 past samples and the present
 * values. A button event is triggered if the newest 4 samples are the same and different from the
 * oldest sample. For example, if button 1 was originally down, button 1 must appear to be up for 4
 * samples before a BUTTON_EVENT_1UP is triggered. This eliminates button bounce, where the button
 * may quickly oscillate between the ON and OFF state as it's being pressed or released.
 *
 * NOTE: This will not work properly without ButtonsInit() being called beforehand.
 * @return A bitwise-ORing of the constants in the ButtonEventFlags enum or BUTTON_EVENT_NONE if no
 *         event has occurred.
 */
uint8_t ButtonsCheckEvents(void)
{
    // Initialize new and returning button events
    new_buttonState = BUTTON_EVENT_NONE;
    ret_buttonState = BUTTON_EVENT_NONE;

    if (BUTTON_STATES()) {
        if (BUTTON_STATES() & BUTTON_STATE_1) {
            new_buttonState |= BUTTON_EVENT_1DOWN;
        } else {
            new_buttonState |= BUTTON_EVENT_1UP;
        }

        if (BUTTON_STATES() & BUTTON_STATE_2) {
            new_buttonState |= BUTTON_EVENT_2DOWN;
        } else {
            new_buttonState |= BUTTON_EVENT_2UP;
        }

        if (BUTTON_STATES() & BUTTON_STATE_3) {
            new_buttonState |= BUTTON_EVENT_3DOWN;
        } else {
            new_buttonState |= BUTTON_EVENT_3UP;
        }

        if (BUTTON_STATES() & BUTTON_STATE_4) {
            new_buttonState |= BUTTON_EVENT_4DOWN;
        } else {
            new_buttonState |= BUTTON_EVENT_4UP;
        }

        if (ctr < 5) {
            buttonStatearray[ctr] = new_buttonState;
            ctr++;
        } else {
            // Button 1
            if (((buttonStatearray[2] & BUTTON_EVENT_1DOWN) &&
                    (buttonStatearray[3] & BUTTON_EVENT_1DOWN) &&
                    (buttonStatearray[4] & BUTTON_EVENT_1DOWN) &&
                    (new_buttonState & BUTTON_EVENT_1DOWN)) &&
                    ((buttonStatearray[1] & BUTTON_EVENT_1DOWN) ||
                    ((new_buttonState & BUTTON_EVENT_1DOWN) !=
                    (buttonStatearray[1] & BUTTON_EVENT_1DOWN)))) {
                ret_buttonState |= BUTTON_EVENT_1DOWN;
            }

            if (((buttonStatearray[2] & BUTTON_EVENT_1UP) &&
                    (buttonStatearray[3] & BUTTON_EVENT_1UP) &&
                    (buttonStatearray[4] & BUTTON_EVENT_1UP) &&
                    (new_buttonState & BUTTON_EVENT_1UP)) &&
                    ((buttonStatearray[1] & BUTTON_EVENT_1UP) ||
                    ((new_buttonState & BUTTON_EVENT_1UP) !=
                    (buttonStatearray[1] & BUTTON_EVENT_1UP)))) {
                ret_buttonState |= BUTTON_EVENT_1UP;
            }

            // Button 2
            if (((buttonStatearray[2] & BUTTON_EVENT_2DOWN) &&
                    (buttonStatearray[3] & BUTTON_EVENT_2DOWN) &&
                    (buttonStatearray[4] & BUTTON_EVENT_2DOWN) &&
                    (new_buttonState & BUTTON_EVENT_2DOWN)) &&
                    ((buttonStatearray[1] & BUTTON_EVENT_2DOWN) ||
                    ((new_buttonState & BUTTON_EVENT_2DOWN) !=
                    (buttonStatearray[1] & BUTTON_EVENT_2DOWN)))) {
                ret_buttonState |= BUTTON_EVENT_2DOWN;
            }

            if (((buttonStatearray[2] & BUTTON_EVENT_2UP) &&
                    (buttonStatearray[3] & BUTTON_EVENT_2UP) &&
                    (buttonStatearray[4] & BUTTON_EVENT_2UP) &&
                    (new_buttonState & BUTTON_EVENT_2UP)) &&
                    ((buttonStatearray[1] & BUTTON_EVENT_2UP) ||
                    ((new_buttonState & BUTTON_EVENT_2UP) !=
                    (buttonStatearray[1] & BUTTON_EVENT_2UP)))) {
                ret_buttonState |= BUTTON_EVENT_2UP;
            }

            // Button 3
            if (((buttonStatearray[2] & BUTTON_EVENT_3DOWN) &&
                    (buttonStatearray[3] & BUTTON_EVENT_3DOWN) &&
                    (buttonStatearray[4] & BUTTON_EVENT_3DOWN) &&
                    (new_buttonState & BUTTON_EVENT_3DOWN)) &&
                    ((buttonStatearray[1] & BUTTON_EVENT_3DOWN) ||
                    ((new_buttonState & BUTTON_EVENT_3DOWN) !=
                    (buttonStatearray[1] & BUTTON_EVENT_3DOWN)))) {
                ret_buttonState |= BUTTON_EVENT_3DOWN;
            }

            if (((buttonStatearray[2] & BUTTON_EVENT_3UP) &&
                    (buttonStatearray[3] & BUTTON_EVENT_3UP) &&
                    (buttonStatearray[4] & BUTTON_EVENT_3UP) &&
                    (new_buttonState & BUTTON_EVENT_3UP)) &&
                    ((buttonStatearray[1] & BUTTON_EVENT_3UP) ||
                    ((new_buttonState & BUTTON_EVENT_3UP) !=
                    (buttonStatearray[1] & BUTTON_EVENT_3UP)))) {
                ret_buttonState |= BUTTON_EVENT_3UP;
            }

            // Button 4
            if (((buttonStatearray[2] & BUTTON_EVENT_4DOWN) &&
                    (buttonStatearray[3] & BUTTON_EVENT_4DOWN) &&
                    (buttonStatearray[4] & BUTTON_EVENT_4DOWN) &&
                    (new_buttonState & BUTTON_EVENT_4DOWN)) &&
                    ((buttonStatearray[1] & BUTTON_EVENT_4DOWN) ||
                    ((new_buttonState & BUTTON_EVENT_4DOWN) !=
                    (buttonStatearray[1] & BUTTON_EVENT_4DOWN)))) {
                ret_buttonState |= BUTTON_EVENT_4DOWN;
            }

            if (((buttonStatearray[2] & BUTTON_EVENT_4UP) &&
                    (buttonStatearray[3] & BUTTON_EVENT_4UP) &&
                    (buttonStatearray[4] & BUTTON_EVENT_4UP) &&
                    (new_buttonState & BUTTON_EVENT_4UP)) &&
                    ((buttonStatearray[1] & BUTTON_EVENT_4UP) ||
                    ((new_buttonState & BUTTON_EVENT_4UP) !=
                    (buttonStatearray[1] & BUTTON_EVENT_4UP)))) {
                ret_buttonState |= BUTTON_EVENT_4UP;
            }

            // Reset array counter to 1
            ctr = 1;

            // Move up array values (pop)
            buttonStatearray[1] = buttonStatearray[2];
            buttonStatearray[2] = buttonStatearray[3];
            buttonStatearray[3] = buttonStatearray[4];
            buttonStatearray[4] = new_buttonState;
        }
    }
    return ret_buttonState; // 4-bit new new button state
}
