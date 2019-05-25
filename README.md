# SimpleShell

A shell program that runs in the command line window and executes simple commands, such as "cd", "ls", etc. It also has a history feature that prints out the history of the commands the user entered. Even if the user exits the shell, the history file is saved, and reopened for reading once the user runs SimpleShell again. There is a known bug with the history feature. Once the user exits by pressing Ctrl + c, the history file is supposed to be saved in the user's home directory. My guess is that since the child process executes the commands while the parent process waits, the parent exits before the child process, leaving no time for the child process to create the history file.

## Compilation

Use the make file by simply typing ```make``` in the command line window while in the project directory. This directory will contain the “src” folder. Make will generate an executable named SimpleShell in the project directory. 

## Execution

To run the program, type ```SimpleShell``` in the command line when you are in the project directory. No arguments needed for this program.

