INC+=inc 
COMM=comm
bin:test.o exec_command.o
	gcc -o bin test.o exec_command.o
test.o:
	gcc -c test/test.c -I $(INC) -I ../$(COMM)/$(INC)
exec_command.o:
	gcc -c src/exec_command.c -I $(INC) -I ../$(COMM)/$(INC)
clean:
	rm -rf bin *.o
	
