BIN=test
BIN:test.o
	gcc -o $(BIN) test.o -levent
test.o:
	gcc -c test.c
clean:
	rm -rf test *.o
