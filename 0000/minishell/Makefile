BIN=minishell
OBJ=main.o parse.o
BIN:$(OBJ)
	gcc -o $(BIN) $(OBJ)
main.o:
	gcc -c main.c -L ./
parse.o:
	gcc -c parse.c -I ./
clean:
	rm -rf $(BIN) $(OBJ)