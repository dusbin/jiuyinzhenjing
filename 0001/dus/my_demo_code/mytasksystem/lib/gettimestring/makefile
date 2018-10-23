INC+=inc 
COMM=comm
bin:test.o gettimestring.o
	gcc -o bin test.o gettimestring.o
test.o:
	gcc -c test/test.c -I $(INC) -I ../$(COMM)/$(INC)
gettimestring.o:
	gcc -c src/gettimestring.c -I $(INC) -I ../$(COMM)/$(INC)
clean:
	rm -rf bin *.o
	
