BIN=sortdemo
.PHONY:BIN clean help debug
BIN:
	go build .
debug:
	go build -gcflags "-N -l" -o sortdemo_debug sort.go
clean:
	rm -rf sortdemo_debug sortdemo
help:
	@echo	"make               #编译"
	@echo	"make clean         #清除"
	@echo   "make debug         #编译调试版"