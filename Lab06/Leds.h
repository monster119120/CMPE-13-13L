/*
 * File: Leds.h
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Modified on February 15, 2016, 4:33 PM
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LEDS_H
#define	LEDS_H

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// **** Define global, module-level, or external variables here ****
int led_val; // contains LED value

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
// suppresses various warnings that we don't need to worry about for CMPE13
#ifndef _SUPPRESS_PLIB_WARNING
#define _SUPPRESS_PLIB_WARNING
#endif

#define LEDS_INIT() do {                                               \
                          TRISECLR = 0x00FF;                           \
                          PORTECLR = 0x00FF;                           \
                          } while (0)                                      
#define LEDS_SET(mask) do {                                            \
                          PORTE = mask;                                \
                        } while (0)                                    

#define LEDS_GET() (led_val = LATE)                          

#endif	/* LEDS_H */
