# ACSL-Compiler
ACSL compiler made in C++. Made in 2022 by Timothy Brua.

This project was created as extra credit for a final exam. I was proud enough of this project to include it
in my resume.

I will give an overview of how this program works below. I expect the reader to know know how ACSL works (mostly).

---------------------

First the program creates a couple of vectors to hold certain information, such as the lines of the program,
and labels and their values. All of these vectors and variables I keep track of in the program are part of the ACSLCompiler class,
which contains all the methods I will be using.

The program then takes in a .txt file with ACSL code, the name of the file is specified by the user. The program 
then counts the number of lines in the file, not including any input (which goes on the last line of the file). The program 
knows when to stop counting after it sees the END opcode. It creates a vector based on the number of lines 
and puts each line into its own index in the vector. So line 1 would go into lines[0], line 2 to lines[1], and so on. Keeping
track of the lines in a vector is useful because I can then use a variable to get a specific line, even if it has already been run.

This variable is called commandLine, which starts at 0 and keeps track of the line the program is running on, this is useful because
the lines[] vector is 0-indexed and in doing so I can call for the exact line simply using the commandLine variable. Also,
the commandLine variable is useful whenever the program loops.

I want to point out a variable called ACC, which stands for Accumulator.

--------------------
.
Getting to the first real part of the code, is the ACSLCompiler::parse(string) function.

This function takes a string as input (a line of code), which it then parses for the LABEL, OPCODE, and LOC (Location)
stated on that line of code. 

I keep track of a variable called 'start' in this function, this variable tells me where I need to start 
the substring to get the next piece of the line. So at the beginning I have not used substring yet, so it start = 0. After
I find the first substring, start will then start at the character AFTER the first space on the line.

Taking a line as example.

A DC 4

word 1, start = 0.
word 2, start = 2.
word 3, start = 5.

Because the next 'word' starts at (A DC 4)[2].

In reality, the code runs many more times in order to find the next space, then sets start equal to index-of-next-space+1.

After the code has found the first space, it substrings the first word, so in our case it would be "A". After the substring,
it immediately checks if the OPCODE has been found, if it has then the program knows whatever it just found must be the LOC. In our case,
the OPCODE has not been found, so we move on to checking the word "A" against all preset OPCODEs. Obviously "A" is not an OPCODE, so we move
on to our last case. If OPCODE has not been set again, even after all the checks, it has found the "A" is a LABEL, so LABEL = "A". It then moves
on to the next word, which would be "DC".

The OPCODE has not been set yet so it checks "DC" against all preset OPCODEs, it finds the DC is an OPCODE, so it sets OPCODE = "DC".

Lastly, it checks "4". Since the OPCODE has been found, the first check goes true, and sets whatever it has left as the LOC, LOC = "4".

After it has parsed the entire line, it then runs a helper function called ACSLCompiler::command(string, string, string).

-------------------------------------

The ACSLCompiler::command(string, string, string) function is much simpler.

The first parameter passed is the OPCODE, which it then checks against all OPCODEs. Once it finds the specific
OPCODE it is looking for, it runs that specific piece of code.

In our previous example:

A DC 4

The OPCODE is "DC", so it matches "DC" with "DC" and runs that code. DC declares a variable (LABEL) with a specific value (LOC).

So in our case the code would then convert "4" into int(4), which it would then (assuming this is the first DC in the file),
put "A" in the labels[] vector at index 0, and int(4) into the variables[] vector at index 0. Each time something is added to either of those vectors, the 
vector is also resized to its current-size+1.

-------------------------

I will go through a simple program as for thorough explanation.

Take
A DC 4
TOP LOAD A
SUB =1
STORE A
PRINT A
BG TOP
END

This program will output:
1: 3
2: 2
3: 1
4: 0

(first number simply counts the number of print statements)

All the program does it count down from 4 until it hits 0.

1st, "A DC 4" sets the label A = 4
2nd, TOP marks that line is a line labeled TOP (which is just the top of the loop), it also LOADs A into the ACC
3rd, "SUB =1" subtracts int(1) from the ACC
4th, it then STOREs the contents of ACC into A, which would be int(3), so A = 3.
5th, it then PRINTs the contents of A, which is int(3).
6th, if ACC > 0 (BG, Greater), then it jumps to TOP, which is line 2
It then repeats 2, 3, 4, 5, and 6, until it reaches 0
7th, of ACC = 0, it will not jump to TOP, but instead go over to END, and terminate the program.
