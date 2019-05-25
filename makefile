GCC=gcc
SOURCE=src
C=*.c
MAIN=SimpleShell

all: project

project:
	@$(GCC) -o $(MAIN) $(SOURCE)/$(C)

clean:
	@rm $(MAIN)
