/*
 * File:   part0.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Created on January 11, 2016, 8:14 PM
 */

// ***Include libraries here***
#include "xc.h"
#include "BOARD.h"

// Prints out "Hello World!" to console
void main(void)
{
    BOARD_Init();
    printf("Hello World\n");
    while(1); // causes program to run infinitely
}
