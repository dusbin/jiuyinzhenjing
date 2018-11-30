CUR_SOURCE=${wildcard ./src/*.c}
#NO_DIR=$(notdir $(CUR_SOURCE))
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}
#all:
#	@echo $(CUR_SOURCE)
#	@echo $(NO_DIR)
#	@echo $(CUR_OBJS)
all:$(CUR_OBJS)
$(CUR_OBJS):%.o:%.c
	$(CC) $(COMFLAGS) ./$^ -o $(ROOT_DIR)/$(OBJS_DIR)/$(notdir $@) $(CFLAGS)
#all:CommSocket.o comm_define.o
#CommSocket.o:
#	$(CC) $(COMFLAGS) ./src/CommSocket.c -o $(ROOT_DIR)/$(OBJS_DIR)/$@ $(CFLAGS)
#comm_define.o:
#	$(CC) $(COMFLAGS) ./src/comm_define.c -o $(ROOT_DIR)/$(OBJS_DIR)/$@ $(CFLAGS)