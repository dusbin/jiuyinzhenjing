BIN=test_log
OBJ= test.o log.o
LIBRARY = liblog.a
BIN:$(OBJ)
	gcc -o $(BIN) $(OBJ) 
LIBRARY:$(OBJ)
	ar cr $(LIBRARY) log.o 
test.o:
	gcc -c test.c -I inc/
log.o:
	gcc -c src/log.c -I inc/
clean:
	rm -rf $(OBJ) $(BIN) $(LIBRARY)
	