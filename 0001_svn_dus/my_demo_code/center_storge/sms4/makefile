CC = gcc
BIN = test
OBJ = test.o \
sms4.o
#LDFLAGS = -lpthread
COMFLAGS = -c -g -w

$(BIN):$(OBJ)
	$(CC) -o $(BIN) $(OBJ) 
test.o:
	$(CC) $(COMFLAGS) test.c 
sms4.o:
	$(CC) $(COMFLAGS) sms4.c 
	
.PHONY:clean
clean:  
	-rm -f $(BIN) $(OBJ)
	
	