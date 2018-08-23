/*
 * File:   main.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Created on January 13, 2016, 10:01 PM
 */

// ***Include libraries here ***
#include "BOARD.h"
#include "CMath.h"
// #include "math.h" -- For hypot and atan2 functions
#include "stdio.h"
#include "stdlib.h"
#include "xc.h"

// If using math.h - declarations of Euclid and arctan functions
// float hypot(float x, float y);
// float atan2(float y, float x);

// If using CMath.h and CMath.c - declarations of Euclid and arctan functions
float arctangent2(float y, float x);
float enorm(float px, float py, float qx, float qy);

// Initial values for Euclidean distance
float px = 2.0;
float py = 1.0;
float qx = 4.0;
float qy = 3.0;

// Initial values for arctangent formula
float x = 4.0;
float y = 4.0;

float result; // Result variable for Euclidean distance
float result_2; // Result variable for arctangent formula

int main(void)
{
    BOARD_Init();

    // Euclidean distance
    printf("This program calculates the Euclidean distance between two points.\n");
    printf("px = %3.5f, py = %3.5f, qx = %3.5f, qy = %3.5f\n", px, py, qx, qy);
    printf("Expected value:           2.82843\n");

    // result = hypot(4.0 - 2.0, 3.0 - 1.0); -- When using math.h and hypot
    result = enorm(2.0, 1.0, 4.0, 3.0); // using CMath.h and CMath.c

    printf("The Euclidean distance is %3.5f\n\n", result);

    // Arctangent formula
    printf("This program calculates the arctangent of two values.\n");
    printf("x = %3.5f, y = %3.5f\n", x, y);
    printf("Expected value:   0.78540\n");

    // result_2 = atan2(4.0, 4.0); -- When using math.h and atan2
    result_2 = arctangent2(4.0, 4.0); // using CMath.h and CMath.c

    printf("The arctangent is %3.5f\n", result_2);

    while (1);

}
