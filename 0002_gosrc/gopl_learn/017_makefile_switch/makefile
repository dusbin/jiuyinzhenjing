BIN=main
.PHONY:tag1 tag2 clean help
tag1:
	go build -tags imp1 -o imp1
tag2:
	go build -tags imp2 -o imp2
clean:
	rm -rf sortdemo_debug sortdemo
help:
	@echo	"make imp1          #flag1"
	@echo	"make imp2          #flag2"
	@echo   "make clean         #清除"