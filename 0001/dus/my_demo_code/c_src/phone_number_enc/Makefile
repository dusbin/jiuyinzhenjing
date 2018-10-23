CC = gcc
BIN = phone_enc
OBJ = phone_number_enc.o uxmd5.o

CFLAGS = -I .
LDFLAGS = -lpthread
COMFLAGS = -c -g -w

$(BIN):$(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LDFLAGS) $(CFLAGS) -ldl
phone_number_enc.o:
	$(CC) $(COMFLAGS) phone_number_enc.c $(LDFLAGS)  -ldl $(CFLAGS)
uxmd5.o:
	$(CC) $(COMFLAGS) uxmd5.c $(LDFLAGS) -ldl $(CFLAGS)
	
.PHONY:clean
clean:  
	-rm -f $(BIN) $(OBJ)