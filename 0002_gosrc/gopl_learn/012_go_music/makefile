BIN=myfunction
.PHONY:BIN clean help
BIN:
	go build .
clean:
	rm -rf myfunction
test:
	@echo	"\033[32mTest library manager \033[0m"
	go test ./library/
help:
	@echo	"make               #编译"