*SJSU, CS 149, Mak*
*Assignment*:6
*Team*: forkbomb
*Members*: Joshua Filstrup, Patrick Roteman, Tim Stullich

*Compiling instructions*:

        on GCC:
            $ g++ -std=c++0x *.cc
            $ ./a.out

        on Clang:
            $ clang++ -std=c++11 -stdlib=libc++ *.cc
            $ ./a.out

We changed the precision of timestamps to be in nanoseconds rather than milliseconds because there was no visible difference between when children wrote a message and the parent read it.

We ran into one main difficulty while working on this assignment. The issue was in setting up the pipes to send multiple messages from the child to parent. The reason this was a problem is because normal anonymous pipes such as shown in class and in most examples online can not be reopened after being closed. Only one message can be sent through those pipes. This makes sense when the main purpose of these pipes is standard in and standard out redirection. That type of redirection only happens once, with one message.

We thought of three different approaches to fix this problem: share memory between processes, create multiple pipes between each pair of children and the parent and discard them as they get used, or create a named pipe between each pair of children and the parent. We thought sharing memory would probably work quite well, but the assignment said to use pipes so we opted to use named pipes instead. 

The parent opens the read end of 5 pipes, and the children repeatedly open the write end, write a message, and then close it. The parent loops through repeated `select()` (actually `pselect()` since that doesn't alter the timeout value from repeated calls) calls to check if each child sent it a message.

We didn't have any issues with getting standard in from child 5 like some groups reported. We also thought using select or pselect was relatively straight forward and very useful in preventing blocking on read actions from the pipe.
