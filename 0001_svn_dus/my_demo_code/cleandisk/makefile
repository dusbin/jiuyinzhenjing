INC = inc
SRC = src
MAIN = main
DB = db
UX_COMM_LIB = ux_comm_lib

CC = gcc
BIN = cleandisk
OBJ = main.o \
sqlite3.o db_interface.o uxmd5.o

CFLAGS = -I $(DB)/$(INC)/ -I $(MAIN)/$(INC)/ -I $(UX_COMM_LIB)/$(INC)/
LDFLAGS = -lpthread
COMFLAGS = -c -g -w

$(BIN):$(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LDFLAGS) $(CFLAGS) -ldl
main.o:
	$(CC) $(COMFLAGS) $(MAIN)/$(SRC)/main.c $(LDFLAGS)  -ldl $(CFLAGS)
sqlite3.o:
	$(CC) $(COMFLAGS) $(DB)/$(SRC)/sqlite3.c $(LDFLAGS) -ldl $(CFLAGS)
db_interface.o:
	$(CC) $(COMFLAGS) $(DB)/$(SRC)/db_interface.c $(LDFLAGS) -ldl $(CFLAGS)
uxmd5.o:
	$(CC) $(COMFLAGS) $(UX_COMM_LIB)/$(SRC)/uxmd5.c $(LDFLAGS) -ldl $(CFLAGS)
	
.PHONY:clean
clean:  
	-rm -f $(BIN) $(OBJ)
	
	