BIN=test
OBJ=test.o log.o
BIN:$(OBJ)
	gcc -o $(BIN) $(OBJ) -levent
test.o:
	gcc -c test.c -I ../log/inc
log.o:
	gcc -c ../log/src/log.c
clean:
	rm -rf test *.o
