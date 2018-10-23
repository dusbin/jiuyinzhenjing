CC = gcc
BIN = test
OBJ = test.o \
des.o
#LDFLAGS = -lpthread
COMFLAGS = -c -g -w

$(BIN):$(OBJ)
	$(CC) -o $(BIN) $(OBJ) 
test.o:
	$(CC) $(COMFLAGS) test.c 
sqlite3.o:
	$(CC) $(COMFLAGS) des.c 
	
.PHONY:clean
clean:  
	-rm -f $(BIN) $(OBJ)
	
	