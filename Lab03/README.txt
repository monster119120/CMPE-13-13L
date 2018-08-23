Sophia Tacderas
CMPE 13L-04, Fri 1-3 pm
Due: 1/27/16

Lab 3 - Matrix Math 
I did not collaborate with anyone on this project.

	The ojbect of this lab was to build a function library for performing
various operations on 3x3 matrices. Such functions included adding/multiplying
matrices with a scalar, adding/multiplying two matrices together, testing for 
equality between two matrices, and finding the determinant and inverse of a
matrix. This lab required the use of for-loops for majority of the functions
and understanding the concepts of function prototype/definition. It was also
necessary to know how to implement arrays correctly and use unit testing for
testing each function (which is hard-coding values and using if-else loops
to test for function success or failure).
	I started by reading the lab manual first to have a general idea of
what is expected of me to code and implement. I coded the easier functions 
first in MatrixMath.c, such as MatrixAdd, MatrixScalarMultiply, 
MatrixScalarAdd, and MatrixPrint. As there was no way of testing equal 
matrices at this point, I did not do my unit testing for those functions
(except for MatrixPrint, which does not require unit testing at all, just
printing out the expected and actual output). I then proceeded to code my
MatrixEquals function in MatrixMath.c. For my functions so far, I realized
I needed to use nested for loops, the top loop being the row counter and the
bottom loop being the column counter, as arrays are generally in row major
form. I had to create two float variables, row and col, and store in the
result matrix (after the column for-loop) the result of adding two matrices,
multiplying a scalar x etc. for each value in the array (in the form
mat[row][col]). I believe the more difficult and long functions were
MatrixEquals, as it required the use of FP_DELTA to check for round-off error;
MatrixMultiply,which required making a third for-loop underneath the first two
for counting the common indices between Matrix 1 and Matrix 2; and MatrixInverse (I did 
this last), which needed the use of three functions, MatrixDeterminant, 
MatrixTranspose, and MatrixScalarMultiply, in order to divide the transpose of
Matrix 1's cofactor matrix (or the adjugate matrix) by Matrix 1's determinant.
My biggest problems were usually round-off error and displaying results
correctly (which I solved by using FP_DELTA and type-casting using doubles),
and trying to test for MatrixInverse; at first I tried using the identity 
matrix to test for it, but it always failed, and, as I eventually learned in
class, it is not a reliable matrix to test with. If I were to approach this
lab again, I would definitely start with the MatrixEquals function first, as
this is needed to unit test majority of the other functions, and start with
the easier functions and unit test each one I finish.
	The lab, which took me two days to finish, is now working
successfully, with no errors or warnings,and showing the correct output
required. I liked being able to apply my understanding of matrices and matrix
operations into C; the only thing I disliked was how long this lab took me to
finish, though it is overall a worthwhile lab. To make this lab better, I
think we should be required to write functions that are guaranteed to provide
more precise output, with less round-off error to concern ourselves with. I
think the point distribution overall is fair, and the lab manual answered 
majority of my questions that I had about the lab. I believe I understood the
concepts well enough for this lab, but I think it would be helpful for other
people to go over them during class a little bit more.
