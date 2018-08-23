Sophia Tacderas
CMPE 13L-04, Fri 1-3 pm
Due: 2/3/16
I did not collaborate with anyone for this lab.

Lab 4: RPN Calculator

	For this lab, I was required to create a calculator that processed
calculations in reverse polish notation (RPN) format. The format consists of
two operands, followed by the operator. For example, (8 4 +) is the
equivalent to (8 + 4) in a normal calculator. This lab required the use of
stacks, structs, function prototypes, unit testing, and new but relevant 
functions such as atof(), strtok(), and strlen(). It was also necessary to
implement the functions of the stack listed in the Stack.h library in Stack.c,
a file I had to create myself. For me, the important aspects of the lab
included learning how structs work, how to implement the functions of a stack
through a struct, how to use strtok() and atof() to parse a string into tokens
and convert characters into operands or have them work as operators, where to
place error messages in nested if/else statements, and the syntax for stacks
and pointers.
	My approach to the lab was the same as when I approached Lab 3: I read
the lab manual to get a general idea of the concepts and what was required of
me, and then followed the iterative design plan. I started by coding the 
functions for Stack.c; what I had the hard time most with during this section
was generally typing the syntax for the struct. I believe there was 
information regarding this in the lab manual; however, I would have also liked
if the lectures that explained this were up on the class website, which were
not. After finishing the unit tests for the functions, I then moved on to
coding rpn.c, which was much more time-consuming than Stack.c and the unit
tests. Again, I followed the iterative design plan for this part of the lab.
How to parse the string into tokens and using the delimiter took some time for
me because I did not follow the syntax for strtok() or typed the delimiter
correctly. I also was stuck on how to properly handle 0.0 as an input, so I
asked Max for help on that. For reading the string for operations (+, - , *,
/) and deciding which operation was entered, I reused the code from Lab2 
(which was also a calculator lab) to perform the operation on the operands. 
For the division operation, I even kept the code that outputs the result as
zero if dividing by zero occurs, which comes from Lab2. My biggest mistake in
this lab was that I added all the sections of my code (pushing/popping 
operands, checking operators, error messages etc.) all at once, without
testing each section. When I ran it on the terminal, nothing was working or
displaying correctly. I solved this by following the design plan one step at
a time, carefully coding each section and  testing it before moving on to the
next one. If I approached this lab the next time, I would definitely be a lot
more careful with posting sections of the lab and just work one section at a 
time; I noticed in the end that there are so many nested if-else statements
I needed to put; the lab, I realized, would have been so hard to do if I added
all my sections at once without testing them and without realizing the actual
order and flow they should follow all throughout.
	My lab, in the end, works correctly as it should. It took me four days
to finish; I liked that it taught me to be much more careful in coding and to
take things one step at a time. What I disliked was the syntax for structs and
arrays; for example, it took me a long time to find out that referencing a
character in a parsed string had the syntax *string == 'char'. I believe there
may have been information somewhere in the lab manual or textbook, but it was
mostly small syntax errors that threw me off for a long time while working on
this lab. I also would have found it more helpful, again, to have the lectures
pertaining to the concepts taught in this lab up on the class website.
Overall, the points distribution is fair. It is a worthwhile lab as long as the
concepts are understood very well and as long as one starts coding early;
based on my experience, it is definitely not advisable to work on this lab 
the last day or two days before it is due.
