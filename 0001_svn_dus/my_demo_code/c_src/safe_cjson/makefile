test:cJSON.o test.o 
	gcc -o test cJSON.o test.o  -lm -g
cJSON.o:
	gcc -c cJSON.c -g
test.o: test.c cJSON.h
	gcc -c test.c -g
clean:
	rm test cJSON.o test.o
