INC = inc
SRC = src
MAIN_DIR = ../main
PLUGIN_MGR = ../plugin_mgr
COMM = ../comm

CC = gcc
BIN = main
OBJ = plugin_mgr.o \
uxmd5.o rbtree.o secure_memory.o

CFLAGS = -I $(COMM)/$(INC)/ -I $(PLUGIN_MGR)/$(INC)/ -I $(MAIN_DIR)/$(INC)/
LDFLAGS = -lpthread
COMFLAGS = -c -g -w

$(BIN):$(OBJ)
	$(CC) -rdynamic -o $(BIN) $(MAIN_DIR)/$(SRC)/main.c $(OBJ) $(LDFLAGS) -ldl $(CFLAGS)
plugin_mgr.o:
	$(CC) $(COMFLAGS) $(PLUGIN_MGR)/$(SRC)/plugin_mgr.c $(LDFLAGS) -ldl $(CFLAGS)
uxmd5.o:
	$(CC) $(COMFLAGS) $(COMM)/$(SRC)/uxmd5.c $(CFLAGS)
rbtree.o:
	$(CC) $(COMFLAGS) $(COMM)/$(SRC)/rbtree.c $(CFLAGS)
secure_memory.o:
	$(CC) $(COMFLAGS) $(COMM)/$(SRC)/secure_memory.c $(CFLAGS)
.PHONY:clean
clean:  
	-rm -f $(BIN) $(OBJ)
