test:cJSON.o test.o
#	gcc -o test cJSON.o test.o -lpthread -ldl -lm -g
	gcc -o test cJSON.o test.o -lm -ldl 
cJSON.o:cJSON.c cJSON.h
	gcc -c cJSON.c
test.o: test.c test.h cJSON.h
	gcc -c test.c
clean:
	rm test cJSON.o test.o
