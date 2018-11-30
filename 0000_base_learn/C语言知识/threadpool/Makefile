BIN=example
obj=thread-pool.o example.o
BIN:$(obj)
	gcc -o $(BIN) $(obj) -I . -lpthread
example.o:
	gcc -c example.c 
thread-pool.o:
	gcc -c thread-pool.c
clean:
	rm -rf $(BIN) $(obj)