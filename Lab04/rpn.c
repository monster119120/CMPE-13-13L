/*
 * File: rpn.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Modified on January 29, 2016, 2:19 PM
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries


// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****
#define STACK_SIZE 20
#define MAX_CHAR 60

// **** Declare function prototypes ****
#include "Stack.h"

/*
 * Function prototype for ProcessBackspaces() - This function should be defined
 * below main after the related comment.
 */
int ProcessBackspaces(char *rpn_sentence);

// If this code is used for testing, rename main to something we can call from our testing code. We
// also skip all processor configuration and initialization code.
#ifndef LAB4_TESTING

int main()
{
    BOARD_Init();
#else

int their_main(void)
{
#endif // LAB4_TESTING

    /******************************** Your custom code goes below here *******************************/

    /*
    struct testStack myStack;
    myStack.stackItems[0] = 10.1;
    myStack.initialized = FALSE;


    // Test StackInit()
    printf("Before StackInit:\n"
            "Current item index = %d\n"
            "Initialized array? = %d\n\n",
            myStack.currentItemIndex, myStack.initialized);

    StackInit(&myStack);
    printf("After StackInit:\n"
            "Current item index = %d\n"
            "Initialized array? = %d\n\n",
            myStack.currentItemIndex, myStack.initialized);


    // Test StackPush() and StackIsFull()
    int i = 0;
    float j = 3.14;
    int current_size;
    for (i = 0; i < (STACK_SIZE + 2); i++) {
        if (StackIsFull(&myStack) == FALSE) {
            StackPush(&myStack, j);
            int indexptr_after = myStack.currentItemIndex;
            printf("After StackPush: %f\n", myStack.stackItems[indexptr_after]);
            current_size = StackGetSize(&myStack);
            printf("Current stack size: %d\n", current_size);
            j = j + 1;
        } else {
            printf("Stack is full.\n\n");
            break;
        }
    }


    // Test StackPop(), StackIsEmpty(), and StackGetSize()
    for (i = (STACK_SIZE + 2); i > 0; i--) {
        if (StackIsEmpty(&myStack) == FALSE) {
            float pop_value = 0;
            StackPop(&myStack, &pop_value);
            printf("After StackPop: %f\n", (double) pop_value);
            current_size = StackGetSize(&myStack);
            printf("Current stack size: %d\n", current_size);
        } else {
            printf("Stack is empty.\n\n");
            break;
        }
    }
     */


    // Declare the Stack struct with new name myStack
    struct Stack myStack;

    // Greeting to user
    printf("Welcome to Sophia's RPN calculator program!\n\n");

    while (1) {
        // Always initialize stack at beginning of while loop
        StackInit(&myStack);

        // Allow user to input a string
        printf("Enter floats and + - * / in RPN format:\n");
        char my_string[61]; // set char array (my_string) up to 61

        // Initialize my_string with nulls
        int s;
        for (s = 0; s < 60; s++) {
            my_string[s] = '\0';
        }

        // Read in an input string using fgets()
        fgets(my_string, MAX_CHAR, stdin);

        // If user's string length is too long, display error
        // Else, parse the string into tokens
        int string_len = strlen(my_string);
        if (string_len == 59) {
            printf("\nError: string length is greater than 60.\n\n");
        } else {
            // Declare char pointer called new_string
            char *new_string;
            // Delimiter separates tokens through spaces, nulls, and linefeeds
            char delimx[3] = " \0\n";
            // Parse the string into tokens via strtok()
            // Move parsed string to new_string
            new_string = strtok(my_string, delimx);

            // Other important variables
            double string_num; // variable that stores atof result
            float result_val = 0.0; // result of RPN calculation
            int error_flag = 0; // becomes "1" for any error displayed in program

            // Continue to read new_string until a null is reached
            while (new_string != NULL) {
                // Convert tokens into a float or operator
                string_num = atof(new_string);

                // If token (string_num) is an operand, push it onto stack
                // Else, stack is full; program errors
                if (string_num != 0.0) {
                    StackPush(&myStack, string_num);
                    if (StackIsFull(&myStack) == TRUE) {
                        printf("Error: Stack is full; too many operands.\n\n");
                        error_flag = 1;
                        break;
                    }
                } else {
                    if ((*new_string == '0') && (string_num == 0.0)) {
                        // This checks if token is equal to zero
                        StackPush(&myStack, string_num);
                        if (StackIsFull(&myStack) == TRUE) {
                            printf("Error: Stack is full; too many operands.\n\n");
                            error_flag = 1;
                            break;
                        }
                    } else {
                        // If token is an operator (+ - * /),
                        // Pop two operands and push result (only if stack is not empty)
                        // Else, one of the error messages below show up
                        float operand_1, operand_2;
                        if (StackIsEmpty(&myStack) == FALSE) {
                            StackPop(&myStack, &operand_2);
                            if (StackIsEmpty(&myStack) == FALSE) {
                                StackPop(&myStack, &operand_1);

                                result_val = 0.0;
                                if (*new_string == '+') {
                                    result_val = (operand_1 + operand_2);
                                    StackPush(&myStack, result_val);
                                    if (StackIsFull(&myStack) == TRUE) {
                                        // Displays if stack is full for result_val
                                        printf("Error: Stack is full; too many operands.\n\n");
                                        error_flag = 1;
                                        break;
                                    }
                                }
                                if (*new_string == '-') {
                                    result_val = (operand_1 - operand_2);
                                    StackPush(&myStack, result_val);
                                    if (StackIsFull(&myStack) == TRUE) {
                                        // Displays if stack is full for result_val
                                        printf("Error: Stack is full; too many operands.\n\n");
                                        error_flag = 1;
                                        break;
                                    }
                                }
                                if (*new_string == '*') {
                                    result_val = (operand_1 * operand_2);
                                    StackPush(&myStack, result_val);
                                    if (StackIsFull(&myStack) == TRUE) {
                                        // Displays if stack is full for result_val
                                        printf("Error: Stack is full; too many operands.\n\n");
                                        error_flag = 1;
                                        break;
                                    }
                                }
                                if (*new_string == '/') {
                                    if (operand_2 == 0.0) {
                                        result_val = 0.0; // print "0" if dividing by zero
                                    } else {
                                        result_val = operand_1 / operand_2;
                                    }
                                    StackPush(&myStack, result_val);
                                    if (StackIsFull(&myStack) == TRUE) {
                                        // Displays if stack is full for result_val
                                        printf("Error: Stack is full; too many operands.\n\n");
                                        error_flag = 1;
                                        break;
                                    }
                                }
                                if ((*new_string != '+') && (*new_string != '-')
                                        && (*new_string != '*') && (*new_string != '/')) {
                                    // Displays if operator entered is not one of the above
                                    printf("Error: invalid character in operand string.\n\n");
                                    error_flag = 1;
                                    break;
                                }
                            } else {
                                // Displays if operand_1 is not present
                                printf("Error: not enough operands before operator.\n\n");
                                error_flag = 1;
                                break;
                            }
                        } else {
                            // Displays if operand_2 is not present
                            printf("Error: not enough operands before operator.\n\n");
                            error_flag = 1;
                            break;
                        }
                    }
                }
                new_string = strtok(NULL, delimx); // puts back nulls in new_string
            }
            if (error_flag == 0) {
                // Only pops result if there are no errors
                StackPop(&myStack, &result_val);
                if (StackIsEmpty(&myStack) == FALSE) {
                    // Displays if operands are still present (stack is not empty)
                    printf("Error: too many operands before one operator.\n\n");
                } else {
                    // Else, display result
                    printf("RPN result: %f\n\n", (double) result_val);
                }
            }
        }
    }


    /*************************************************************************************************/

    // You can never return from main() in an embedded system (one that lacks an operating system).
    // This will result in the processor restarting, which is almost certainly not what you want!
    while (1);
}

/**
 * Extra Credit: Define ProcessBackspaces() here - This function should read through an array of
 * characters and when a backspace character is read it should replace the preceding character with
 * the following character. It should be able to handle multiple repeated backspaces and also
 * strings with more backspaces than characters. It should be able to handle strings that are at
 * least 256 characters in length.
 * @param rpn_sentence The string that will be processed for backspaces. This string is modified in
 *                     place.
 * @return Returns the size of the resultant string in `rpn_sentence`.
 */
int ProcessBackspaces(char *rpn_sentence)
{
    return 0;
}
