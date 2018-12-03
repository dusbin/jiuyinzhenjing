BIN=myfunction
.PHONY:BIN clean help
BIN:
	go build -gcflags "-N -l" -o test . #关闭编译器代码优化和函数内联，可以避免断点和单步执行无法准确对应代码行，避免小函数和局部变量被优化掉
clean:
	rm -rf test
help:
	@echo	"make               #编译"