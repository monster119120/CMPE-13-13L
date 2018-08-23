Sophia Tacderas
CMPE 13L-04, Fri 1-3 pm
Due: 3/10/16 at 11:59 pm

*****I emailed Max for a 24-hour extension. I have only an hour left but my
partner, Bowen Brooks, has all his late days, so we are using his.
Submitted: 3/11/16 at 11:58 pm

Lab 9: BattleBoats

	My name is Sophia Tacderas and I worked with Bowen Brooks on this lab.
I worked on Field and Bowen worked on Protocol. Neither of us worked with
anyone else on this lab (other than asking Max for help).
	In this lab, the objective was to write code that would allow two Uno32
kits connected to each other to play Battleship. With a partner, we were 
required to work on two libraries separately: Field, which has functions that
control the BattleBoats field through Field.c; and Protocol, which describes
the communication protocol via Protocol.c. Together, however, both partners
implement the artificial agent via ArtificialAgent.c. There is another agent
for the human player that is in an object file, and with BattleBoats.c, the
program is ready to run. Important concepts included the use of hashing and
checksums, encryption/decryption, finite state machines, and the function 
rand(). Because this is the first and only partner lab in the class, being able
to code independently and together was key in this lab.
	Like all my previous labs, I read/re-read the lab manual and watched the
lecture videos if I didn't completely understand anything. Over the weekend, 
both of us worked on our own libraries by ourselves. I did not think my library
was too difficult to implement; the function requirements and parameters were
rather straightforward. What Bowen and I had a hard time with was working on
ArtificialAgent.c. Our primary problem at first was that our challenge message
was not generating correctly, which did not allow us to get past the 
AGENT_STATE_GENERATE_NEG_DATA state; talking to Max allowed us to solve that
problem by creating an if(in) statement above the state machine, and putting
pStatus = ProtocolDecode(in, &EnemyData, &enemyGuess); inside the brackets. We
also had to ask Max as to why our guess and hash data were not showing up 
correctly. To fix this, we had to fix our ProtocolGenerateNegotiationData()
and ProtocolValidateNegotiationData() functions, as we were not XORing the hash
value with the correct bits: we had to XOR it with the guess and 0xFF
ANDED together, shiftt the result by 8 bits, and then XOR the guess ANDED with
0xFF00. Bowen and I mostly added printf() to our state machine to see what
worked in the state machine and what did not. We also printed out values of
variables and checked in the IDE what the values were. To me, this felt like
a good way to approach the lab and I do not think I would approach it any 
differently. I really enjoyed working with a partner because I thought it was
nice to be able to program and have someone overlook your work and catch any
mistakes/errors I was unable to notice. In the future, I believe this lab should
be kept as a partner lab.
	In the end, we were only able to get as far as encoding the determine
message. The program is unfinished though it took us since Saturday to figure
things out. It seems like the results look incredibly interesting, though
I wish there was a design plan (the lab would be so much more worth it if we had
one). I think there were details about how the messages/checksums/guesses/hashes
were received that were not gone over in the lab manual. The grading for this
lab seems approriate overall. I am at least thankful this is not the last lab
assigned to us, but I think this lab takes more than a week to complete.
