Sophia Tacderas
CMPE 13L-04, Fri 1-3 pm
Due: 3/2/16, 11:59 pm

*****I am submitting this lab late.
Submitted: 3/3/16, 7:49 pm
I have 1 hour, 10 min left in my time bank.

For Lab 9, Battleboats, my partner is Bowen Brooks.
I did not collaborate with anyone for this lab, Lab 8.

Lab 8: Morse Code

	In this lab, we used event-driven programming, finite state machines
(FSMs), and a timer interrupt to input Morse code, a series of dots and 
dashes, and translate this code into letters and words. Important C concepts
included the malloc() function, pointers, recursion, and, mostly, binary 
trees. Our job was to write code so that when we pressed button 4, a short 
press would display a dot on the top line of the Oled, and a long press would
display a dash on the top line. Once one was finished with inputting the Morse
code, the translated letter would show on the bottom line.
	As usual, I approached this lab by reading the lab manual and built 
the project folder on MPLABX before working on the lab itself. I decided to 
look at a couple of the lecture video recordings before starting on this lab
as well to review the concepts of binary trees and recursion. When I started
coding, I began with implementing the Tree.h and Morse.h functions, which I
think were not that difficult, compared to lab8.c. Despite giving myself more
time to work on this lab, as I started on Saturday/Sunday, I am submitting this
lab one day late again, on a Thursday. The biggest challenge of this lab was
that the program was not being able to detect the inter letter and inter word
events in my state machine in main(). After hours and almost days of having 
this problem, I solved it this morning  by reversing the order of the states
from dot, dash, interletter, and interword to inteword, interletter, dash, and
dot (basically, just reversing the states by placing them from greatest amount
of ticks to least amount of ticks). I had this problem and went to office hours
hoping I would be able to have this problem fixed, but it did not happen.
	The final results work correctly as shown in the demo video. I spent
about 5-6 days on this lab. I overall liked being able to implement Morse code
on the Oled and translate it, and I liked how this lab felt easier to handle,
as we were only supposed to keep track of one button event, rather than focus
on multiple buttons, switches, and the pot. It was also interesting to use the
binary tree for this lab. I disliked how it took me so long to finish a lab
that I thought was rather simple in objective; it turned out to take longer
than I thought it would; I think having an iterative design plan would probably
have saved me more time. I think the points distrubution overall seems 
appropriate. Although I wish there was a design plan in the lab manual, the
manual seems to go into much detail with the new concepts such as the binary
trees and recursion; however, I did notice an error: on page 8, in the unit 
test, below the comment // Now change the button state to trigger an UP_EVENT
at t = 60, in the for statement, i should be less than 52, not 32. I caught
this error because an up event should last 52 ticks, according to the diagram
on page 7, and I noticed my unit test was not going past that part of code (I
had printfs after each if statement to show that the unit test made it past
each part). I think I understood the concepts well enough in class when we 
first learned it, although I believe we could have spent 10 more minutes in
lecture talking about the lab (we only spent the first 10 minutes talking about
it on Tuesday).
