// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries


// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****
#define M_PI 3.14159265358979323846 // measure of pi

// **** Declare function prototypes ****
double AbsoluteValue(double op1);
double FahrenheitToCelsius(double op1);
double CelsiusToFahrenheit(double op1);
double Average(double op1, double op2);
double Tangent(double op1);
double Round(double operand);

// If this code is used for testing, rename main to something we can call from our testing code.
#ifndef LAB2_TESTING

int main(void)
{
    BOARD_Init();
#else

int their_main(void)
{
#endif // LAB2_TESTING

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/

    // Greeting to user
    printf("Welcome to my calculator program!\n\n");

    /* Give user a list of operations
       Allow user to input their choice */
    char input; // stores operator in form of char
    int flag = 0; // flags for valid/invalid operators
    while (1) {
        printf("Enter a character to perform "
                "one of the following operations:\n");
        printf("(+, -, *, /, a, c, f, v, t, r)\n");
        scanf(" %c", &input);

        // Check for valid/invalid operators
        if ((input == '+') || (input == '-') || (input == '*')
                || (input == '/') || (input == 'a') || (input == 'c')
                || (input == 'f') || (input == 'v') || (input == 't')
                || (input == 'r')) {
            flag = 1; // valid operator entered
        } else {
            printf("\n\nInvalid operator %c. Try again.\n", input);
            flag = 0; // invalid operator entered
        }

        // Allow user to enter operand(s)
        if (flag == 1) {
            double operand1 = 0, operand2 = 0;
            printf("\nEnter operand 1: ");
            scanf("%f", &operand1);
            /* For operations that require two operands 
               (+, -, *, /, v) */
            if ((input == '+') || (input == '-') || (input == '*')
                    || (input == '/') || (input == 'v')) {
                printf("Enter operand 2: ");
                scanf("%f", &operand2);
            }

            // Operator functions
            double result = 0;
            if (input == '+') {
                result = operand1 + operand2;
            } else if (input == '-') {
                result = operand1 - operand2;
            } else if (input == '*') {
                result = operand1 * operand2;
            } else if (input == '/') {
                if (operand2 == 0) {
                    result = 0; // print "0" if dividing by zero
                } else {
                    result = operand1 / operand2;
                }
            } else if (input == 'a') {
                result = AbsoluteValue(operand1);
            } else if (input == 'c') {
                result = CelsiusToFahrenheit(operand1);
            } else if (input == 'f') {
                result = FahrenheitToCelsius(operand1);
            } else if (input == 'v') {
                result = Average(operand1, operand2);
            } else if (input == 't') {
                result = Tangent(operand1);
            } else if (input == 'r') {
                result = Round(operand1);
            }

            // Print result of operations here
            if ((input == '+') || (input == '-') || (input == '*')
                    || (input == '/') || (input == 'v')) {
                printf("\nThe result of %f %c %f is %f.\n\n",
                        operand1, input, operand2, result);
                // For binary operations
            } else {
                printf("\nThe result using %f for operation %c is %f.\n\n",
                        operand1, input, result);
                // For unary operations
            }
            flag = 0; // reset flag for next calculation
        }
    }


    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     *****************************************************************************/
    while (1);
}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double AbsoluteValue(double op1)
{
    if (op1 < 0) {
        return op1 *= -1;
        // multiply op1 by -1 if op1 is neg
    } else {
        return op1;
        // otherwise return op1 as is
    }
}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double FahrenheitToCelsius(double op1)
{
    // Fahr-Celsius conversion formula
    op1 = ((5.0 / 9.0) * (op1 - 32.0));
    return op1;
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double CelsiusToFahrenheit(double op1)
{
    // Celsius-Fahr conversion formula
    op1 = (((9.0 / 5.0) * op1) + 32.0);
    return op1;
}

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double Average(double op1, double op2)
{
    // Add both ops and divide by 2
    op1 = ((op1 + op2) / 2);
    return op1;
}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees (Note: Your tangent 
 * function relies on a call from the tangent function of math.h which uses 
 * radians).
 ********************************************************************************/
double Tangent(double op1)
{
    /* rad = op1 in radians
       rad_result = result of tan(rad); 
       (in radians) */
    double rad, rad_result;

    // Convert op1 from degrees to radians
    // M_PI = measure of pi (declared below libraries)
    rad = ((op1 * M_PI) / (180));

    // Use rad in the tangent function of math.h
    rad_result = tan(rad);
    return rad_result;
}

/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
double Round(double operand)
{
    double operand_int = 0; // integer counter of operand
    double operand_dec = operand; // copy operand_dec into operand

    // If operand_dec is neg, obtain its absolute value
    if (operand_dec < 0) {
        operand_dec = operand_dec * (-1);
    }

    /* Continuously decrement operand_dec by 1
       as well as increment operand_int by 1
       until operand_dec is greater than or equal to 1 */
    while (operand_dec >= 1) {
        operand_dec = operand_dec - 1;
        ++operand_int;
    }

    /* Once operand_dec >= 0 AND < 1
       Add 1 to operand_int
       if operand_dec is greater than 0.5 */
    if (operand_dec > 0.5) {
        ++operand_int;
    }

    /* If operand was originally neg
       multiply -1 to operand_int and return it */
    if (operand < 0) {
        operand_int = operand_int * (-1);
    }
    return operand_int;
}
