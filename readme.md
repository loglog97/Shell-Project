# Shell Project for CMPE 320
## by: Logan Cole - Spring 2022

#### Reference for project
[Ostep projects](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/processes-shell)

#### About the project
The point of the project was to build a simple Unix based shell. Along with learning how processes are created, destroyed, and managed. From working on the project, I learned:
* How to deal more with pointers and arrays in C
* How forking works and how to use it efficiently
* Dealing with strings in C compared to other high-level languages
* How bash operates behind the scenes

## What the different files in the project are used for

The code can be compiled running
```
make wish
```

From there you can run by either:
```
./wish - for interactive mode
./wish <somefile>.txt - for batch mode
```
#### Wish.c
This is the main file of the program. You are able to either run it interactivly like a normal command line, or writing little shell scripts. From here is where it decides how it will handle the input it is given.

#### commmandParser.c
Once a command or a line is given, this is where it is sent to start being delt with. If we are given parrallel commands separated by an '&', we also handle this here. We then parse an individual command to then be sent to be handled.

#### built_in.c
This is where the magic happens. Depending on what type of command we are given determines how it is delt with in here. We have 3 built in commands. If we have a command that isn't built in, it is handled by searching through the paths we have to attempt to execute. The three commands that are built in are:
* path
* cd
* exit