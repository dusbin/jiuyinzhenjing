BIN=minishell
OBJ=main.o parse.o
CC=gcc
CFLAGS=-Wall -g
$(BIN):$(OBJ)
	$(CC) $(CFLAGS) $^ -o $@
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -rf $(BIN) $(OBJ)