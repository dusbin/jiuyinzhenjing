BIN=test
BIN:test.o rbtree.o
	gcc -o $(BIN) test.o rbtree.o
test.o:
	gcc -c test.c
rbtree.o:
	gcc -c src/rbtree.c
clean:
	rm -rf test *.o
