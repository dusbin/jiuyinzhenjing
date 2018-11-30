test:cJSON.o test.o License.o
	gcc -o test cJSON.o test.o  License.o -lm -g
cJSON.o:
	gcc -c cJSON.c -g
test.o: test.c cJSON.h
	gcc -c test.c -g
License.o:
	gcc -c License.c -g
clean:
	rm test cJSON.o test.o  License.o
