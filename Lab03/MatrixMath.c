/*
 * File:   MatrixMath.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Created on January 22, 2016, 2:18 PM
 */

// ***Include libraries here ***
#include "MatrixMath.h"
#include "stdio.h"
#include "stdlib.h"

// **** Define global, module-level, or external variables here ****
int row, col; // variables for row and column
float x; // define scalar variable

/**
 * MatrixMultiply performs a matrix-matrix multiplication operation on two 3x3
 * matrices and returns the result in the third argument.
 */
void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    int loop_ctr = 0; // is common to mat1 and mat2

    for (row = 0; row < 3; row++) { // Counts through rows of mat1
        for (col = 0; col < 3; col++) { // Counts through cols of mat2
            for (loop_ctr = 0; loop_ctr < 3; loop_ctr++) { // Counts through same vals in mat1/mat2
                result[row][col] = result[row][col] + (mat1[row][loop_ctr] * mat2[loop_ctr][col]);
                // Multiplies each row of mat1 by each col of mat2; result stored in result array
            }
        }
    }
    return;
}

/**
 * MatrixAdd performs a matrix addition operation on two 3x3 matrices and 
 * returns the result in the third argument.
 */
void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    for (row = 0; row < 3; row++) { // Counts through rows of mat1
        for (col = 0; col < 3; col++) { // Counts through cols of mat2
            result[row][col] = mat1[row][col] + mat2[row][col];
            // Add each corresponding value from both arrays; results stored in result array            
        }
    }
    return;
}

/**
 * MatrixEquals checks if the two matrix arguments are equal. The result is
 * 0 if FALSE and 1 if TRUE to follow the standard C conventions of TRUE and
 * FALSE.
 */
int MatrixEquals(float mat1[3][3], float mat2[3][3])
{
    int result; // returns 1 if mat1 and mat2 are equal, 0 if not
    float difference; // diff of each corresponding value in mat1 and mat2

    for (row = 0; row < 3; row++) { // Counts through rows of mat1
        for (col = 0; col < 3; col++) { // Counts through cols of mat2
            difference = mat1[row][col] - mat2[row][col];
            // obtain diff of each corresponding value in mat1 and mat2
            if ((difference < FP_DELTA) && (difference > (FP_DELTA * -1))) {
                result = 1;
            } else {
                result = 0;
            }
            // FP_DELTA = 0.0001
            // return "1" if diff < FP_DELTA AND diff > -FP_DELTA (otherwise "0")

            if (result == 0) {
                break; // break out of loop if result = 0
            }
        }
        if (result == 0) {
            break; // break out of loop if result = 0
        }
    }
    return result;
}

/**
 * MatrixScalarMultiply performs the multiplication of a matrix by a scalar.
 * The result is returned in the third argument.
 */
void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3])
{
    for (row = 0; row < 3; row++) { // Counts through rows of mat
        for (col = 0; col < 3; col++) { // Counts through cols of mat
            result[row][col] = mat[row][col] * x;
            // Multiply scalar x to each value of array; results stored in result array            
        }
    }
    return;
}

/**
 * MatrixScalarAdd performs the addition of a matrix by a scalar. The result
 * is returned in the third argument.
 */
void MatrixScalarAdd(float x, float mat[3][3], float result[3][3])
{
    for (row = 0; row < 3; row++) { // Counts through rows of mat
        for (col = 0; col < 3; col++) { // Counts through cols of mat
            result[row][col] = mat[row][col] + x;
            // Add scalar x to each value of array; results stored in result array            
        }
    }
    return;
}

/**
 * MatrixDeterminant calculates the determinant of a matrix and returns the
 * value as a float.
 */
float MatrixDeterminant(float mat[3][3])
{
    /* detA = (a00*a11*a22) + (a01*a12*a20) + (a02*a10*a21)
            - (a20*a11*a02) - (a21*a12*a00) - (a22*a10*a01) */

    float det_result = 0.0;

    det_result = (mat[0][0] * mat[1][1] * mat[2][2]) + (mat[0][1] * mat[1][2] * mat[2][0])
            + (mat[0][2] * mat[1][0] * mat[2][1]) - (mat[2][0] * mat[1][1] * mat[0][2])
            - (mat[2][1] * mat[1][2] * mat[0][0]) - (mat[2][2] * mat[1][0] * mat[0][1]);

    return det_result;
}

/**
 * MatrixTrace calculates the trace of a matrix. The result is returned as a
 * float.
 */
float MatrixTrace(float mat[3][3])
{
    float sum = 0.0; // Stores result of a11 + a22 + a33

    for (row = 0; row < 3; row++) { // Counts through rows of mat
        for (col = 0; col < 3; col++) { // Counts through cols of mat
            if (row == col) {
                sum = sum + mat[row][col]; // performs a11 + a22 + a33
            }
        }
    }
    return sum;
}

/**
 * MatrixTranspose calculates the transpose of a matrix and returns the
 * result through the second argument
 */
void MatrixTranspose(float mat[3][3], float result[3][3])
{
    for (row = 0; row < 3; row++) { // Counts through rows of mat
        for (col = 0; col < 3; col++) { // Counts through cols of mat
            if (row == col) {
                result[row][col] = mat[row][col]; // keep [r][c] if r = c 
            } else {
                result[row][col] = mat[col][row]; // otherwise switch to [c][r]
            }
        }
    }
    return;
}

/**
 * MatrixInverse calculates the inverse of a matrix and returns the
 * result through the second argument.
 */
void MatrixInverse(float mat[3][3], float result[3][3])
{
    // Stores adjugate matrix of mat[3][3]
    float adjugate[3][3] = {
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}
    };
    // Stores transpose of adjugate matrix
    float adjugate_trans[3][3] = {
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}
    };

    // Stores inverse of determinant of mat[3][3]]
    float determ_inv = 0.0;

    // Add in formulas for adjugate matrix
    adjugate[0][0] = (mat[1][1] * mat[2][2]) - (mat[1][2] * mat[2][1]); // b
    adjugate[0][1] = (mat[1][2] * mat[2][0]) - (mat[1][0] * mat[2][2]); // c
    adjugate[0][2] = (mat[1][0] * mat[2][1]) - (mat[1][1] * mat[2][0]); // d

    adjugate[1][0] = (mat[0][2] * mat[2][1]) - (mat[0][1] * mat[2][2]); // e
    adjugate[1][1] = (mat[0][0] * mat[2][2]) - (mat[0][2] * mat[2][0]); // f
    adjugate[1][2] = (mat[0][1] * mat[2][0]) - (mat[0][0] * mat[2][1]); // g

    adjugate[2][0] = (mat[0][1] * mat[1][2]) - (mat[0][2] * mat[1][1]); // h
    adjugate[2][1] = (mat[0][2] * mat[1][0]) - (mat[0][0] * mat[1][2]); // k
    adjugate[2][2] = (mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]); // l

    // Transpose adjugate matrix, store in adjugate_trans
    MatrixTranspose(adjugate, adjugate_trans);

    // Get (1) / (determinant of mat) and store in variable
    determ_inv = ((1) / (MatrixDeterminant(mat)));

    // Scalar-multiply (1/determinant) and transposed adjugate matrix
    // result = inverse matrix of mat
    MatrixScalarMultiply(determ_inv, adjugate_trans, result);

    return;
}

/**
 * MatrixPrint sends a 3x3 array to standard output with clean formatting.
 * The formatting does not need to look like the expected output given to you
 * in MatrixMath.c but each element of the matrix must be separated and have
 * distinguishable position (more than a single line of output).
 */
void MatrixPrint(float mat[3][3])
{
    for (row = 0; row < 3; row++) { // Counts through rows of mat
        printf("\n------------------------------------\n");
        for (col = 0; col < 3; col++) { // Counts through cols of mat
            printf("| ");
            printf("%0.4f  ", (double) mat[row][col]);
            printf(" |");
        }
    }
    printf("\n------------------------------------");
    return;
}
