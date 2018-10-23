BIN=test
OBJ= test.o
BIN:$(OBJ)
	gcc -o $(BIN) $(OBJ)
test.o:
	gcc -c test.c
clean:
	rm -rf $(OBJ) $(BIN)