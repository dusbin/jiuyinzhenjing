BIN=main
.PHONY:server client clean help
help:
	@echo	"make client        #服务端"
	@echo	"make server        #客户端"
	@echo   "make clean         #清除"
server:
	go build -tags server -o server
client:
	go build -tags client -o client
clean:
	rm -rf server client
