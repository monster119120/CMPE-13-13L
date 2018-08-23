/*
 * File: toaster_oven.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Modified on February 20, 2016, 8:09 PM
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CMPE13 Support Library
#include "BOARD.h"
#include "Adc.h"
#include "Ascii.h"
#include "Buttons.h"
#include "Oled.h"
#include "OledDriver.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Leds.h"

// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timers, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)

// **** Declare any datatypes here ****

enum {
    RESET_STATE,
    START_STATE,
    COUNTDOWN_STATE,
    PENDING_SELECTOR_CHANGE_STATE,
    PENDING_RESET_STATE
} Oven_State = RESET_STATE;

struct Oven_State_Data {
    uint16_t oven_state; // Oven State
    uint16_t init_cook_time; // Initial Cook Time
    uint16_t cook_time_left; // Cooking Time Left
    uint16_t temp; // Temperature
    char *cook_mode; // Cooking Mode - Bake, Toast, Broil
    uint16_t button_press_ctr; // Button Press Counter
    char *input_select; // Input Selection - Time, Temp
};

// **** Define global, module-level, or external variables here ****
struct Oven_State_Data myOven_State_Data;
static char OledString[100]; // argument of OledDrawString()

#define TOP_OVEN_ON 0x01
#define TOP_OVEN_OFF 0x02
#define BOT_OVEN_ON 0x03
#define BOT_OVEN_OFF 0x04
#define DEG_SYMBOL 0xF8
#define LED_ALL_ON 0xFF

#define TEMP_DEFAULT 300
#define BROIL_TEMP_DEFAULT 500
#define TIME_DEFAULT 0

#define ARROW ">"
#define SPACE " "
#define BAKE "Bake "
#define BROIL "Broil"
#define TOAST "Toast"
#define TIME "TIME"
#define TEMP "TEMP"
#define ZERO 0
#define TWO 2
#define THREE 3

#define LOOP_STRING(x) x,x,x,x,x

#define LONG_PRESS 5

static char *time_marker;
static char *temp_marker;

static uint8_t first_time = TRUE;
static uint8_t adc_event = FALSE;
static uint8_t oven_on_flag = FALSE;
static uint16_t adc_read = 0;
static uint16_t adc_time = 0;
static uint16_t adc_temp = 0;

static uint16_t display_min = 0;
static uint16_t display_sec = 0;
static uint16_t button_ctr = 0;
static uint8_t button_event;
static uint16_t free_Running_Timer = 0;
static uint16_t TwoHz_Countup_Timer = 0;
static uint8_t TwoHz_timer_flag = FALSE;

// Used for LED
static uint16_t LED_total_clock = 0;
static uint16_t LED_clear_interval = 0;
static uint16_t LED_bit_ctr = 0;
static uint8_t LED_mask = 0xFF;
static uint8_t LED_off = 0x00;

// Configuration Bit settings

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
void display_OLED();
void display_LED();

/*******************************************************************************
 * MAIN FUNCTION                                                               *
 ******************************************************************************/
int main()
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR1 to F_PB / 256 / 2 yields a 0.5s timer.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, BOARD_GetPBClock() / 256 / 2);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_256, BOARD_GetPBClock() / 256 / 5);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T3);
    INTSetVectorPriority(INT_TIMER_3_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_3_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T3, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    // Initialize Oled, LEDs, Adc, and Buttons
    OledInit();
    LEDS_INIT();
    AdcInit();
    ButtonsInit();

    while (1) {
        // Unit Test
        //        myOven_State_Data.cook_mode = BAKE;
        //        myOven_State_Data.cook_time_left = 16;
        //        myOven_State_Data.temp = 350;
        //        myOven_State_Data.oven_state = COUNTDOWN;
        //        time_marker = ARROW;
        //        temp_marker = SPACE;

        //        myOven_State_Data.cook_mode = TOAST;
        //        myOven_State_Data.cook_time_left = 25;
        //        myOven_State_Data.temp = 350;
        //        myOven_State_Data.oven_state = COUNTDOWN;
        //        time_marker = SPACE;
        //        temp_marker = SPACE;

        //        myOven_State_Data.cook_mode = BROIL;
        //        myOven_State_Data.cook_time_left = 25;
        //        myOven_State_Data.temp = 350;
        //        myOven_State_Data.oven_state = START;
        //        time_marker = SPACE;
        //        temp_marker = ARROW;

        //        display_OLED();

        // Get current Oven State from struct if not the first time

        if (first_time == TRUE) {
            first_time = FALSE;
        } else {
            Oven_State = myOven_State_Data.oven_state;
        }

        //**********************************************************************
        //                          Finite State Machine
        //**********************************************************************
        switch (Oven_State) {
        case RESET_STATE:
            //**************************************************************
            // NO EVENT
            // NEXT STATE = START_STATE
            //**************************************************************
            LEDS_SET(LED_off);
            LED_bit_ctr = ZERO;
            LED_mask = LED_ALL_ON;

            //Set Input selection for time
            myOven_State_Data.input_select = TIME;

            //Reset temp to default
            myOven_State_Data.temp = TEMP_DEFAULT;

            ///Turn oven off 
            oven_on_flag = FALSE;

            if (!myOven_State_Data.cook_mode) {
                // Default to Bake mode
                myOven_State_Data.cook_mode = BAKE;
            }
            myOven_State_Data.init_cook_time = TIME_DEFAULT;
            myOven_State_Data.cook_time_left = TIME_DEFAULT;

            time_marker = ARROW;
            temp_marker = SPACE;

            // Update the display
            display_OLED();

            // Change to next State
            myOven_State_Data.oven_state = START_STATE;
            break;

        case START_STATE:
            //**************************************************************
            // POTENTIOMETER CHANGED EVENT
            // NEXT STATE = START_STATE
            //**************************************************************
            if (adc_event == TRUE) {
                adc_read = AdcRead();

                // Update Time or Temp
                if (strcmp(myOven_State_Data.input_select, TIME) == 0) {
                    adc_time = (((adc_read) & (~0xFF00)) + 1);
                    myOven_State_Data.init_cook_time = adc_time;
                    myOven_State_Data.cook_time_left = adc_time;
                    myOven_State_Data.input_select = TIME;
                    time_marker = ARROW;
                    temp_marker = SPACE;
                } else {
                    if (strcmp(myOven_State_Data.input_select, TEMP) == 0) {
                        adc_temp = (((adc_read) & (~0xFF00)) + 300);
                        myOven_State_Data.temp = adc_temp;
                        myOven_State_Data.input_select = TEMP;
                        time_marker = SPACE;
                        temp_marker = ARROW;
                    }
                }

                // Update the display
                display_OLED();

                // Clear ADC event
                adc_event = FALSE;

                // Change to next State
                // No change in state
                myOven_State_Data.oven_state = START_STATE;
            }
            //**************************************************************
            // BUTTON_EVENT_3DOWN EVENT
            // NEXT STATE = PENDING_SELECTOR_CHANGE_STATE
            //************************************************************m**
            if (button_event & BUTTON_EVENT_3DOWN) {
                // Store Free Runing Timer to struct
                myOven_State_Data.button_press_ctr = free_Running_Timer;

                // Clear button event
                button_event = FALSE;

                // Change to next State
                myOven_State_Data.oven_state = PENDING_SELECTOR_CHANGE_STATE;
            }

            //**************************************************************
            // BUTTON_EVENT_4DOWN EVENT
            // NEXT STATE = COUNTDOWN_STATE
            //**************************************************************
            if (button_event & BUTTON_EVENT_4DOWN) {
                // Change to next State
                myOven_State_Data.oven_state = COUNTDOWN_STATE;

                // Reset 2Hz timer 
                TIMER_2HZ_RESET();

                // Double the value because of 2Hz clock
                // 2 Hz clock 1 tick = .5 sec, 2 ticks 1 sec
                // Set initial start time
                TwoHz_Countup_Timer = ZERO;

                // Set 2Hz timer flag
                TwoHz_timer_flag = FALSE;

                //************** initial values for LEDs Set Here
                // This will be the actual cooking time measured in 2Hz for LEDs
                LED_clear_interval = ((myOven_State_Data.cook_time_left) >> THREE);
                LED_total_clock = (myOven_State_Data.cook_time_left - LED_clear_interval);

                // Set All LEDs to 1 (initial vale of LED_mask s FF)
                LEDS_SET(LED_mask);

                // Turn oven on   
                oven_on_flag = TRUE;

                // Update the display
                display_OLED();

                // Clear button event
                button_event = FALSE;
            }
            break;

        case COUNTDOWN_STATE:
            //**************************************************************
            // 2HZ TIMER TRIGGERED && COOKING TIME LEFT > 0 EVENT    
            // NEXT STATE = COUNTDOWN_STATE (No Change in state)
            //**************************************************************
            if ((TwoHz_timer_flag) && (myOven_State_Data.cook_time_left > 0)) {
                // Change to next State 
                // No Change in state
                myOven_State_Data.oven_state = COUNTDOWN_STATE;

                // Cooking time left -= 1
                myOven_State_Data.cook_time_left -= 1;

                // Update the display
                display_OLED();

                // Update the LEDS
                if ((myOven_State_Data.cook_time_left <= LED_total_clock) &&
                        (myOven_State_Data.cook_time_left > 0)) {
                    if (LED_total_clock >= ZERO) {
                        display_LED();
                        if (LED_total_clock < LED_clear_interval) {
                            LED_total_clock = ZERO;
                        } else {
                            LED_total_clock -= LED_clear_interval;
                        }
                    }
                }

                // Clear 2Hz timer flag
                TwoHz_timer_flag = FALSE;
            }
            //**************************************************************
            //  BUTTON_EVENT_4DOWN EVENT     
            //  NEXT STATE = PENDING_RESET_STATE
            //**************************************************************
            if (button_event & BUTTON_EVENT_4DOWN) {
                // Store free running timer
                myOven_State_Data.button_press_ctr = free_Running_Timer;

                // Clear button event
                button_event = FALSE;

                // Change to next State 
                myOven_State_Data.oven_state = PENDING_RESET_STATE;
            }
            //**************************************************************
            // 2HZ TIMER TRIGGERED && COOKING TIME LEFT == 0 EVENT  
            // NEXT STATE = RESET_STATE
            //**************************************************************
            if ((TwoHz_timer_flag) && (myOven_State_Data.cook_time_left == 0)) {
                // Update the display
                display_OLED();

                // Clear 2Hz timer flag
                TwoHz_timer_flag = FALSE;

                // Change to next State 
                myOven_State_Data.oven_state = RESET_STATE;
            }
            break;

        case PENDING_SELECTOR_CHANGE_STATE:
            // Compute for difference of free running timer and 
            // stored button press ctr 
            button_ctr = (free_Running_Timer - myOven_State_Data.button_press_ctr);
            //**************************************************************
            // BUTTON COUNTER >= LONG PRESS EVENT
            // NEXT STATE = START_STATE
            //**************************************************************
            if (button_ctr >= LONG_PRESS) {
                myOven_State_Data.oven_state = START_STATE;

                //Switch input selection
                if (strcmp(myOven_State_Data.cook_mode, BAKE) == 0) {
                    if (strcmp(myOven_State_Data.input_select, TIME) == 0) {
                        myOven_State_Data.input_select = TEMP;
                    } else {
                        myOven_State_Data.input_select = TIME;
                    }
                } else {
                    myOven_State_Data.input_select = TIME;
                }

                //Update time or temp from pot
                adc_read = AdcRead();

                // Check if Input Select is Time or Temp
                // Use strcmp as stored value is string "TIME" or "TEMP"
                if (strcmp(myOven_State_Data.input_select, TIME) == 0) {
                    adc_time = (((adc_read) & (~0xFF00)) + 1);
                    myOven_State_Data.init_cook_time = adc_time;
                    myOven_State_Data.cook_time_left = adc_time;
                    myOven_State_Data.input_select = TIME;
                    time_marker = ARROW;
                    temp_marker = SPACE;
                } else {
                    if (strcmp(myOven_State_Data.input_select, TEMP) == 0) {
                        adc_temp = (((adc_read) & (~0xFF00)) + 300);
                        myOven_State_Data.temp = adc_temp;
                        myOven_State_Data.input_select = TEMP;
                        time_marker = SPACE;
                        temp_marker = ARROW;
                    }
                }
            }
            //**************************************************************
            // RESET BUTTON COUNTER < LONG PRESS and BUTTON_EVENT_3UP EVENT
            //  NEXT STATE = START_STATE
            //**************************************************************
            if (button_ctr < LONG_PRESS) {
                printf("SHORT_PRESSgoober\n");
                if (button_event & BUTTON_EVENT_3UP) {
                    myOven_State_Data.oven_state = START_STATE;

                    //Reset time & temp, input selection
                    //Rotate cooking mode
                    if (strcmp(myOven_State_Data.cook_mode, BAKE) == 0) {
                        // Bake  --> Toast
                        myOven_State_Data.cook_mode = TOAST;
                        time_marker = SPACE;
                        temp_marker = SPACE;
                        myOven_State_Data.input_select = TIME;
                        myOven_State_Data.init_cook_time = TIME_DEFAULT;
                        myOven_State_Data.cook_time_left = TIME_DEFAULT;
                    } else {
                        if (strcmp(myOven_State_Data.cook_mode, TOAST) == 0) {
                            // Toast --> Broil
                            myOven_State_Data.cook_mode = BROIL;
                            myOven_State_Data.temp = BROIL_TEMP_DEFAULT;
                            time_marker = SPACE;
                            temp_marker = SPACE;
                            myOven_State_Data.input_select = TIME;
                            myOven_State_Data.init_cook_time = TIME_DEFAULT;
                            myOven_State_Data.cook_time_left = TIME_DEFAULT;
                        } else if (strcmp(myOven_State_Data.cook_mode, BROIL) == 0) {
                            // Broil --> Bake
                            myOven_State_Data.cook_mode = BAKE;
                            myOven_State_Data.temp = TEMP_DEFAULT;
                            time_marker = ARROW;
                            temp_marker = SPACE;
                            myOven_State_Data.input_select = TIME;
                            myOven_State_Data.init_cook_time = TIME_DEFAULT;
                            myOven_State_Data.cook_time_left = TIME_DEFAULT;
                        }
                    }
                }
            }

            // Update the display
            display_OLED();

            // Clear button event
            button_event = FALSE;

            // Change to next State
            // Same next state
            break;

        case PENDING_RESET_STATE:
            //**************************************************************
            // BUTTON_EVENT_4UP EVENT
            //  NEXT STATE = COUNTDOWN_STATE
            //**************************************************************
            if (button_event & BUTTON_EVENT_4UP) {
                // Clear button event
                button_event = FALSE;

                // Change to next State
                myOven_State_Data.oven_state = COUNTDOWN_STATE;
            }
            //**************************************************************
            // RESET BUTTON COUNTER  >= LONG_PRESS  EVENT
            //  NEXT STATE = RESET_STATE
            //**************************************************************
            button_ctr = (free_Running_Timer - myOven_State_Data.button_press_ctr);
            if (button_ctr >= LONG_PRESS) {
                // Change to next State
                myOven_State_Data.oven_state = RESET_STATE;
            }
            //**************************************************************
            // 2HZ TIMER TRIGGERED && COOKING TIME LEFT > 0 EVENT    
            // NEXT STATE = PENDING_RESET_STATE (No Change in state)
            //**************************************************************
            if ((TwoHz_timer_flag) && (myOven_State_Data.cook_time_left > 0)) {
                // Cooking time left -= 1
                myOven_State_Data.cook_time_left -= 1;

                // Update the display
                display_OLED();

                // Clear 2Hz timer flag 
                TwoHz_timer_flag = FALSE;

                // Change to next State 
                // No Change in state
                myOven_State_Data.oven_state = PENDING_RESET_STATE;
            }
            //**************************************************************
            // 2HZ TIMER TRIGGERED && COOKING TIME LEFT == 0 EVENT  
            // NEXT STATE = RESET_STATE
            //**************************************************************
            if ((TwoHz_timer_flag) && (myOven_State_Data.cook_time_left == 0)) {
                // Clear 2Hz timer flag
                TwoHz_timer_flag = FALSE;

                // Change to next State 
                myOven_State_Data.oven_state = RESET_STATE;
            }
            break;
        }
    }
    while (1);

}

// This function populates the OLED based on cooking mode

void display_OLED()
{
    // OLED 4 lines 25 char width

    // Bake --> Mode Time Temp
    // Toast --> Mode Time
    // Broil --> Mode Time Temp

    // Compute for display time
    // COUNTDOWN AND PENDING_RESET STATES use Cooking time left
    if ((myOven_State_Data.oven_state == COUNTDOWN_STATE) ||
            (myOven_State_Data.oven_state == PENDING_RESET_STATE)) {
        display_sec = myOven_State_Data.cook_time_left;

        // Splits up time left into minutes and seconds
        display_min = (myOven_State_Data.cook_time_left / 60);
        display_sec = (myOven_State_Data.cook_time_left % 60);

    } else {
        // States other than COUNTDOWN OR PENDING_RESET STATES use Cooking time 
        display_sec = myOven_State_Data.init_cook_time;

        // Splits up time left into minutes and seconds
        display_min = (myOven_State_Data.cook_time_left / 60);
        display_sec = (myOven_State_Data.cook_time_left % 60);
    }

    // BAKE
    if (strcmp(myOven_State_Data.cook_mode, BAKE) == 0) {
        if (oven_on_flag == TRUE) {
            sprintf(OledString, "|%c%c%c%c%c|\t Mode: %s\n|     |\t%sTime: %u:%02u\n"
                    "|-----|\t%sTemp: %3u%cF\n|%c%c%c%c%c|",
                    LOOP_STRING(TOP_OVEN_ON), myOven_State_Data.cook_mode, time_marker,
                    display_min, display_sec, temp_marker, myOven_State_Data.temp,
                    DEG_SYMBOL, LOOP_STRING(BOT_OVEN_ON));
        } else {
            sprintf(OledString, "|%c%c%c%c%c|\t Mode: %s\n|     |\t%sTime: %u:%02u\n"
                    "|-----|\t%sTemp: %3u%cF\n|%c%c%c%c%c|",
                    LOOP_STRING(TOP_OVEN_OFF), myOven_State_Data.cook_mode, time_marker,
                    display_min, display_sec, temp_marker, myOven_State_Data.temp,
                    DEG_SYMBOL, LOOP_STRING(BOT_OVEN_OFF));
        }
    }

    // TOAST
    if (strcmp(myOven_State_Data.cook_mode, TOAST) == 0) {
        // Toast does not need selection markers
        time_marker = SPACE;
        temp_marker = SPACE;

        if (oven_on_flag == TRUE) {
            sprintf(OledString, "|%c%c%c%c%c|\t Mode: %s\n|     |\t%sTime: %u:%02u\n"
                    "|-----|              \n|%c%c%c%c%c|",
                    LOOP_STRING(TOP_OVEN_OFF), myOven_State_Data.cook_mode, time_marker,
                    display_min, display_sec, LOOP_STRING(BOT_OVEN_ON));
        } else {
            sprintf(OledString, "|%c%c%c%c%c|\t Mode: %s\n|     |\t%sTime: %u:%02u\n"
                    "|-----|              \n|%c%c%c%c%c|",
                    LOOP_STRING(TOP_OVEN_OFF), myOven_State_Data.cook_mode, time_marker,
                    display_min, display_sec, LOOP_STRING(BOT_OVEN_OFF));
        }
    }

    // BROIL
    if (strcmp(myOven_State_Data.cook_mode, BROIL) == 0) {
        time_marker = SPACE;
        temp_marker = SPACE;
        if (oven_on_flag == TRUE) {
            sprintf(OledString, "|%c%c%c%c%c|\t Mode: %s\n|     |\t%sTime: %u:%02u\n"
                    "|-----|\t%sTemp: %3u%cF\n|%c%c%c%c%c|",
                    LOOP_STRING(TOP_OVEN_ON), myOven_State_Data.cook_mode, time_marker,
                    display_min, display_sec, temp_marker, myOven_State_Data.temp,
                    DEG_SYMBOL, LOOP_STRING(BOT_OVEN_OFF));

        } else {
            sprintf(OledString, "|%c%c%c%c%c|\t Mode: %s\n|     |\t%sTime: %u:%02u\n"
                    "|-----|\t%sTemp: %3u%cF\n|%c%c%c%c%c|",
                    LOOP_STRING(TOP_OVEN_OFF), myOven_State_Data.cook_mode, time_marker,
                    display_min, display_sec, temp_marker, myOven_State_Data.temp,
                    DEG_SYMBOL, LOOP_STRING(BOT_OVEN_OFF));

        }
    }

    OledDrawString(OledString);
    OledUpdate();

    return;
}

// This function turns off LED bit by bit from RIGHT to LEFT
// Initial on of all LEDs (xFF) done in START state BUTTON_EVENT_4DOWN

void display_LED()
{

    LED_mask ^= 1 << LED_bit_ctr; // bit shift start at position 0
    LEDS_SET(LED_mask);
    LED_bit_ctr++;
    return;
}

/***************************************************************************************************
 * Your code goes in between this comment and the preceding one with asterisks *
 ***************************************************************************************************/

void __ISR(_TIMER_1_VECTOR, ipl4auto) TimerInterrupt2Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 4;

    if (TwoHz_Countup_Timer >= TWO) {
        TwoHz_timer_flag = TRUE;
        TwoHz_Countup_Timer = ZERO;
    } else {
        TwoHz_timer_flag = FALSE;
        TwoHz_Countup_Timer = TwoHz_Countup_Timer + 1;
    }

    if (AdcChanged() == TRUE) {
        adc_event = TRUE;
    } else {
        adc_event = FALSE;
    }

}

void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;

    free_Running_Timer++;
}

void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    // Call button event function in Buttons.h
    button_event = ButtonsCheckEvents();
}
