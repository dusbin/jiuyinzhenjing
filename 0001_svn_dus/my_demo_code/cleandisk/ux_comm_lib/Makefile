CUR_SOURCE=${wildcard ./src/*.c}
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}
all:$(CUR_OBJS)
$(CUR_OBJS):%.o:%.c
	$(CC) $(COMFLAGS) ./$^ -o $(ROOT_DIR)/$(OBJS_DIR)/$(notdir $@) $(CFLAGS)
