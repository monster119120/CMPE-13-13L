Sophia Tacderas
CMPE 13L-04, Fri 1-3 pm
Due: 2/24/16, 11:59 pm

******I am submitting this assignment late.
Submitted: 2/25/16, 7:00 pm
I have 21 hr  left in my time bank.

Lab 7: Toaster Oven

	For this lab, I did not collaborate with anyone. The only person I 
asked for help was Max during his office hours on Wednesday. We were 
introduced to finite state machines (FSMs) through this lab, which are used
prinarily in hardware systems. An FSM is drawn out like a flowchart, which 
shows each of the certain states required, what happens during each state,
and how all the states are connected. In order to execute all these states
correctly, we were also introduced to the switch statement, which runs a state
according to its "case." Again, this lab is based on event-driven programming
and we were required to keep track of up- and down-events that occurred during
button pressing.
	This lab was particularly difficult and intimidating, having the new
concept of FSMs introduced to me. Once I figured out that I simply had to 
implement the chart specified on page 8 in the lab manual, then I found that 
was what the switch statement was for. Again, I printed out the lab manual, 
read it, and read the handout about event-driven programming online. It was 
not that hard to hard-code the oven template (as seen on page 10 in the lab
manual), but at first I had trouble having my program distinguish between
a short or long press. I also struggled with the interrupts. In the end, I 
realize it is not necessary and should work without it, but Max added a line
called printf("SHORT_PRESSgoober\n"); on line 434, after my if statement for 
checking if, during the PENDING_SELECTOR_CHANGE state, there was a short 
up-event for button 3. For some reason, it actually works with executing
this event; without the line, button 3 only recognizes long presses. If I were
to approach this lab differently, again, I realize I need to take more time
out of my schedule to finish this. I also would like to collaborate with 
students who have had more experience in my class or go to more lab sections
for help. To be honest, I find it unhelpful that Max only has office hours on
Friday and Wednesday, which are the days that the lab is out and when the lab
is due. I would find it more helpful if the office hours could be scheduled
during the middle of the time that the lab is due.
	In the end, my lab mostly works correctly. Starting from Saturday/
Sunday up until today, Thursday, it took me about 5-6 days. The output in the 
end looks interesting to experiment with, although when I spin the pot, the 
time/temp  do not always come out correctly (the beginning and end, though,
are correct, such as 300-555 F and 0:00-4:16 min). Also, the LEDs do not always
equally turn off if the time is not divisible by 8. What I disliked was that
I was stuck on one part for a really long time; in my COUNTDOWN_STATE, I
did not use an "if" statement to check for my 2Hz timer flag and that the
cooking time left was zero. As a result, when pressing button 4, the countdown
only lasted one second and then stopped and went to the reset state. It may
have been a worthwhile lab if I had given myself more time. I think the points
distribution is fair and I think I was given enough material to finish this 
lab. Maybe more discussion in class would help, but I think I just needed more
time. I think this is a good lab to use in future classes.
