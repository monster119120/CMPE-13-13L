/*
 * File:   mml.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Modified on January 24, 2016, 12:20 PM
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"


// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries


// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****
#include "MatrixMath.h"

int main()
{
    BOARD_Init();

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/

    /* These matrices are used for unit testing */
    // Matrix 1
    float mat1[3][3] = {
        {1.10, 2.20, 3.30},
        {4.40, 5.50, 6.60},
        {7.70, 8.80, 9.90}
    };
    // Matrix 2
    float mat2[3][3] = {
        {9.90, 8.80, 7.70},
        {6.60, 5.50, 4.40},
        {3.30, 2.20, 1.10}
    };
    // Matrix 3
    float mat3[3][3] = {
        {1.10, 2.20, 3.30},
        {4.40, 5.50, 6.60},
        {7.70, 8.80, 9.90}
    };
    // Matrix 4
    float mat4[3][3] = {
        {1.1002, 2.20, 3.30},
        {4.40, 5.50, 6.60},
        {7.70, 8.80, 9.90}
    };
    // Matrix 5
    float mat5[3][3] = {
        {1.1002, 2.20, 3.30},
        {4.40, 5.50, 6.60},
        {7.70, 8.80, 9.90}
    };
    // Matrix 6
    float mat6[3][3] = {
        {2.10, 2.20, 3.30},
        {4.40, 5.50, 6.60},
        {7.70, 8.80, 9.90}
    };
    // Matrix 7
    float mat7[3][3] = {
        {3.123, 1.223, 4.566},
        {23.454, 4.667, -10.13},
        {4.552, 8.34, 18.1341}
    };
    // Matrix 8
    float mat8[3][3] = {
        {3.233, 23.44, 1.0005},
        {-1.005, 23.123, 5.31},
        {3.44, 2.334, 2.334}
    };

    // Result Matrix
    float result[3][3] = {
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}
    };
    // Result Matrix #2
    float result_2[3][3] = {
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}
    };

    float x = 2.05, y = 3.22; // Scalar variables for MatrixScalar tests
    int int_result = 0; // Stores result (as int 0 or 1) of MatrixEquals

    int function_pass = 0; // Counts function tests that pass
    int function_fail = 0; // Counts function tests that fail

    int overall_pass = 0; // Counts overall functions that pass
    int overall_fail = 0; // Counts overall functions that fail
    float overall_percent = 0; // Stores percentage of total functions that passed


    // ************************ Test MatrixMultiply(); ************************
    // Expected result of Matrix 1 * Matrix 2
    float res1_mult[3][3] = {
        {36.30, 29.04, 21.78},
        {101.64, 83.49, 65.34},
        {166.98, 137.94, 108.9}
    };

    // Expected result of Matrix 2 * Matrix 1
    float res2_mult[3][3] = {
        {108.9, 137.94, 166.98},
        {65.34, 83.49, 101.64},
        {21.78, 29.04, 36.30}
    };

    MatrixMultiply(mat1, mat2, result);
    if (MatrixEquals(result, res1_mult) == 1) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    MatrixMultiply(mat2, mat1, result_2);
    if (MatrixEquals(result_2, res2_mult) == 1) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    if (function_fail == 0) {
        ++overall_pass;
        printf("\nPASSED 2/2: MatrixMultiply()");
    } else {
        ++overall_fail;
        printf("\nFAILED %d/2: MatrixMultiply()", function_fail);
    }


    // *************************** Test MatrixAdd(); *****************************
    /* Reset function counters every time
        a new function must be tested */
    function_pass = 0;
    function_fail = 0;

    // Expected result of Matrix 1 + Matrix 2
    float res1_add[3][3] = {
        {11.00, 11.00, 11.00},
        {11.00, 11.00, 11.00},
        {11.00, 11.00, 11.00}
    };

    // Expected result of Matrix 1 + Matrix 6
    float res2_add[3][3] = {
        {3.20, 4.40, 6.60},
        {8.80, 11.00, 13.20},
        {15.40, 17.60, 19.80}
    };

    MatrixAdd(mat1, mat2, result);
    if (MatrixEquals(result, res1_add) == 1) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    MatrixAdd(mat1, mat6, result_2);
    if (MatrixEquals(result_2, res2_add) == 1) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    if (function_fail == 0) {
        ++overall_pass;
        printf("\nPASSED 2/2: MatrixAdd()");
    } else {
        ++overall_fail;
        printf("\nFAILED %d/2: MatrixAdd()", function_fail);
    }


    // *************************** Test MatrixEquals(); **************************
    function_pass = 0;
    function_fail = 0;

    // Test equality between Matrices 1 and 3
    int_result = MatrixEquals(mat1, mat3);
    if (int_result == 1) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    // Test equality between Matrices 4 and 5
    int_result = MatrixEquals(mat4, mat5);
    if (int_result == 1) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    if (function_fail == 0) {
        ++overall_pass;
        printf("\nPASSED 2/2: MatrixEquals()");
    } else {
        ++overall_fail;
        printf("\nFAILED %d/2: MatrixEquals()", function_fail);
    }


    // ******************* Test MatrixScalarMultiply(); **************************
    function_pass = 0;
    function_fail = 0;

    // Expected output of Mat1 * x
    float res1_mscalar[3][3] = {
        {2.255, 4.51, 6.765},
        {9.02, 11.275, 13.53},
        {15.785, 18.04, 20.295}
    };

    // Expected output of Mat2 * y
    float res2_mscalar[3][3] = {
        {31.878, 28.336, 24.794},
        {21.252, 17.71, 14.168},
        {10.626, 7.084, 3.542}
    };

    MatrixScalarMultiply(x, mat1, result);
    if (MatrixEquals(result, res1_mscalar) == 1) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    MatrixScalarMultiply(y, mat2, result_2);
    if (MatrixEquals(result_2, res2_mscalar) == 1) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    if (function_fail == 0) {
        ++overall_pass;
        printf("\nPASSED 2/2: MatrixScalarMultiply()");
    } else {
        ++overall_fail;
        printf("\nFAILED %d/2: MatrixScalarMultiply()", function_fail);
    }


    // *************************** Test ScalarAdd(); *****************************
    function_pass = 0;
    function_fail = 0;

    // Expected output of Mat1 + x
    float res1_ascalar[3][3] = {
        {3.15, 4.25, 5.35},
        {6.45, 7.55, 8.65},
        {9.75, 10.85, 11.95}
    };

    // Expected output of Mat2 + y
    float res2_ascalar[3][3] = {
        {13.12, 12.02, 10.92},
        {9.82, 8.72, 7.62},
        {6.52, 5.42, 4.32}
    };

    MatrixScalarAdd(x, mat1, result);
    if (MatrixEquals(result, res1_ascalar) == 1) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    MatrixScalarAdd(y, mat2, result_2);
    if (MatrixEquals(result_2, res2_ascalar) == 1) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    if (function_fail == 0) {
        ++overall_pass;
        printf("\nPASSED 2/2: MatrixScalarAdd()");
    } else {
        ++overall_fail;
        printf("\nFAILED %d/2: MatrixScalarAdd()", function_fail);
    }


    // ************************* Test MatrixDeterminant(); *************************
    function_pass = 0;
    function_fail = 0;

    float det_mat7 = MatrixDeterminant(mat7);
    if (det_mat7 == 747.729492) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    float det_mat8 = MatrixDeterminant(mat8);
    if (det_mat8 == 535.630920) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    if (function_fail == 0) {
        ++overall_pass;
        printf("\nPASSED 2/2: MatrixDeterminant()");
    } else {
        ++overall_fail;
        printf("\nFAILED %d/2: MatrixDeterminant()", function_fail);
    }


    // *************************** Test MatrixTrace(); *************************
    function_pass = 0;
    function_fail = 0;
    float trace = 0.0; // Stores sum of trace function

    trace = MatrixTrace(mat1);
    if (trace == 16.5) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    trace = MatrixTrace(mat5);
    if (trace == 16.5002) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    if (function_fail == 0) {
        ++overall_pass;
        printf("\nPASSED 2/2: MatrixTrace()");
    } else {
        ++overall_fail;
        printf("\nFAILED %d/2: MatrixTrace()", function_fail);
    }


    // ************************ Test MatrixTranspose(); ***************************
    function_pass = 0;
    function_fail = 0;

    // Matrix 1 (Transposed)
    float mat1_trans[3][3] = {
        {1.10, 4.40, 7.70},
        {2.20, 5.50, 8.80},
        {3.30, 6.60, 9.90}
    };
    // Matrix 2 (Transposed)
    float mat2_trans[3][3] = {
        {9.90, 6.60, 3.30},
        {8.80, 5.50, 2.20},
        {7.70, 4.40, 1.10}
    };

    MatrixTranspose(mat1, result);
    if (MatrixEquals(result, mat1_trans) == 1) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    MatrixTranspose(mat2, result);
    if (MatrixEquals(result, mat2_trans) == 1) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    if (function_fail == 0) {
        ++overall_pass;
        printf("\nPASSED 2/2: MatrixTranspose()");
    } else {
        ++overall_fail;
        printf("\nFAILED %d/2: MatrixTranspose()", function_fail);
    }


    // ************************ Test MatrixInverse(); *****************************
    function_pass = 0;
    function_fail = 0;

    // Inverse of Matrix 7
    float inverse_mat1[3][3] = {
        {0.2262, 0.0213, -0.0451},
        {-0.6305, 0.0479, 0.1855},
        {0.2332, -0.0274, -0.0189}
    };

    // Inverse of Matrix 8
    float inverse_mat2[3][3] = {
        {0.0776, -0.0978, 0.1892},
        {0.0385, 0.0077, -0.0339},
        {-0.1529, 0.1365, 0.1835}
    };

    MatrixInverse(mat7, result);
    if (MatrixEquals(result, inverse_mat1) == 1) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    MatrixInverse(mat8, result);
    if (MatrixEquals(result, inverse_mat2) == 1) {
        ++function_pass;
    } else {
        ++function_fail;
    }

    if (function_fail == 0) {
        ++overall_pass;
        printf("\nPASSED 2/2: MatrixInverse()");
    } else {
        ++overall_fail;
        printf("\nFAILED %d/2: MatrixInverse()", function_fail);
    }


    // *************************** Overall Function Count *************************
    overall_percent = ((double) (overall_pass) / (9)) * (100);
    printf("\n\n--------------------------------");
    printf("\n%d out of 9 functions passed (%0.2f%%).",
            overall_pass, (double) overall_percent);


    // *************************** Test MatrixPrint(); ****************************
    printf("\n\nExpected output of MatrixPrint:\n");
    printf("------------------------------------\n");
    printf("| 1.1000   || 2.2000   || 3.3000   |\n");
    printf("------------------------------------\n");
    printf("| 4.4000   || 5.5000   || 6.6000   |\n");
    printf("------------------------------------\n");
    printf("| 7.7000   || 8.8000   || 9.9000   |\n");
    printf("------------------------------------\n");

    printf("\nOutput of MatrixPrint:");
    MatrixPrint(mat1);

    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     *****************************************************************************/

    // Returning from main() is bad form in embedded environments. So we sit and spin.
    while (1);
}
