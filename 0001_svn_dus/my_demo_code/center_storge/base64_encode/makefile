bin:test.o EncodeBase64.o
	gcc -o bin test.o EncodeBase64.o
test.o:
	gcc -c test.c -g 
EncodeBase64.o:
	gcc -c EncodeBase64.c -g
clean:
	rm -rf bin test.o EncodeBase64.o