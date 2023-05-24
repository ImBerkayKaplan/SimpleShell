# SimpleShell

This repository is a shell program that runs in the terminal and executes simple commands. It also can print out the history of the commands the user entered. Even if the user exits the shell, the history file is saved, and reopened for reading once the user runs SimpleShell again. 

## Running The Project In Linux

Use the make file by typing ```make``` in the terminal while in the project's root directory. Make will generate an executable named ```main``` in a ```bin``` directory. 

To run the program, type ```bin/main``` in the terminal when you are in the project's root directory.

## Main Features

The main features of the shell program is the ability to keep a history of the commands. The program can print the history to the screen via the ```h``` and ```history``` commands. A user can also set the number of previous commands displayed in the history via the ```sethistory <number-of-commands>``` command. 

A user can also run the most recent command via the ```rr``` command and even choose a command from the history to run via the ```r <command-index>``` command. The ```<command-index>``` parameter is the unique number inside the history that is assigned to each command. It is visible when the user prints the history to the screen.

The shell also implements its own cd and supports special commands, such as ```cd ..```, ```cd ~```, and ```cd <folder-name>```. The user can change their working directory with the ```cd``` command.

The shell program supports most other commands except the ```|```, ```<```, and ```>``` operations. Future work can focus on adding those symbols to the tokenizer as it currently doesn't parse them. 
