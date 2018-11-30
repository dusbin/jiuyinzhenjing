BIN=main
.PHONY:clean help client server
help:
	@echo	"make client        #client"
	@echo	"make server        #server"
	@echo   "make clean         #清除"
client:
	go build clientf/client.go
server:
	go build serverf/server.go
clean:
	rm -rf server client
