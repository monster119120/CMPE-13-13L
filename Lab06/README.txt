Sophia Tacderas
CMPE 13L-04, Fri 1-3 pm
Due: 2/18/16 at 12 noon  - we received an extension due to Wed's power outage
***I am submitting this lab late using my late days.***
Submittal date: Fri, 2/19/16, 8:15 pm
I have about 1 day, 16 hrs left for my late days.

Lab 6: Bounce

	For this lab I worked by myself; I did not collaborate with anyone.
The only person I talked to in order to get help for this lab was Max. My
partner for Lab 9, Battleboats, is Bowen Brooks.
	This lab introduced us to programming hardware (via the Uno32 kit) and
event-driven programming. New concepts such as timers and ISRs (interrupt
service routines) were introduced. Compared to other labs, this lab allowed us
to physically see the output through LEDs and the Oled screen and control the
output through buttons, switches, and the potentiometer. It was also necessary
to control the buttons/swtiches by bit manipulation, which allows a user to
toggle a certain bit of their choice.
	At first, I thought the lab was difficult because when I read the lab
manual, the instructions were not presented in a straightforward manner, and
some sections, such as the program flow/pseudocode and ISRs, provided vague
information. The first two parts of the lab were definitely easier than the
third, for me, although I struggled with how to implement the interrupt at
first, being a new concept I had to take some time to understand. For part 2,
my Oled was not lighting up at all, even though I felt I coded in the correct
Oled.h functions in main. I needed to talk to Max during office hours and he
explained to me what I was missing (I did not include a character array in my
argument for OledDrawString()). He also helped me with correctly displaying my
potentiometer values, such as with zero-padding. If I were to do this lab
again, I would definitely start earlier. The reason why I started late was
because of a conference I attended at UC Davis the weekend before the lab was
due; therefore, I had to start on Monday, use my extra late days, and finish
today, on Friday. For the next lab, which I already predict to be challenging,
I hope to start as soon as Saturday or Sunday.
	In the end, I was able to correctly implement parts 1 and 2, but not
part 3. For part 3, everything is correct except for how I implemented the UP
events for the buttons, which I am still unsure as to exactly how they are
supposed to be done correctly. In the future, I think part 3 in general needs
more clarification as to how the up- and down-events should be coded. My
output for part 3 lights up the correct LEDs for the corresponding buttons,
but do not keep the previous states (the previous LEDs turn off when a new
button is pressed). Overall, being able to program hardware and to physically
see the results is something that really interests me (me being a computer
engineering major after all), but I think the concepts and pseudocode for all
three parts of this lab need more clarification. Other than what I previously
said, this is a worthwhile lab, and the grading/points distribution are fair,
though it is time-consuming and needs to be started early.
