#不能使用gcc
BIN=test
obj=test.o
COMFLAGS= -lstdc++
BIN:$(obj)
	g++ -o $(BIN) $(obj) 
test.o:
	g++ -c test.cpp
clean:
	rm -rf test *.o