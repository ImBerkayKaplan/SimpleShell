GCC=gcc
SOURCE=src
BIN=bin
C_FILES=sig_handler.c file_error_checks.c tokenize.c history_management.c command_management.c main.c
OBJS=$(patsubst %.c,$(BIN)/%.o,$(C_FILES))
MAIN=$(BIN)/main

all: $(BIN) project

$(BIN):
	mkdir -p $(BIN)

$(BIN)/%.o: $(SOURCE)/%.c
	$(GCC) -c $< -o $@

project: $(OBJS)
	$(GCC) -o $(MAIN) $(OBJS)

clean:
	@rm -rf $(BIN)
