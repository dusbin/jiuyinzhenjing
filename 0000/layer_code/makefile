BIN=bin
OBJ=obj
OBJS = server.o switch.o gettime.o getuser.o getversion.o
CLIENT=$(BIN)/client_bin
SERVER=$(BIN)/server_bin
CFLAGS = -I service/switch/inc/ -I service/business/gettime/inc/ -I service/business/getuser/inc/ -I service/business/getversion/inc/
CC=gcc
.PHONY:server client clean_obj help clean_all
server:$(OBJS)
	$(CC) -o $(SERVER) $(OBJS) $(CFLAGS)
server.o:
	$(CC) -c -g  service/socket/src/server.c $(CFLAGS)
switch.o:
	$(CC) -c -g  service/switch/src/switch.c $(CFLAGS)
gettime.o:
	$(CC) -c -g  service/business/gettime/src/gettime.c $(CFLAGS)
getuser.o:
	$(CC) -c -g  service/business/getuser/src/getuser.c $(CFLAGS)
getversion.o:
	$(CC) -c -g  service/business/getversion/src/getversion.c $(CFLAGS)
client:
	$(CC) -o $(CLIENT) client/client.c
clean_obj:
	rm -rf *.o
clean:
	rm -rf $(BIN)/*
clean_all:
	rm -rf *.o $(BIN)/*
help:
	@echo "make clean		#清理可执行程序"
	@echo "make clean_obj		#清理可OBJ"
	@echo "make clean_all		#清理全部"
	@echo "make server		#编译server"
	@echo "make client		#编译client"
	@echo "make clean;make clean_obj;make server;make client"