BIN=myfunction
.PHONY:BIN install clean help
BIN:
	go build .
install:
	cp -f myfunction /bin/
	ln -s /bin/myfunction /bin/myclient
	ln -s /bin/myfunction /bin/myserver
clean:
	rm -rf /bin/myclient /bin/myserver /bin/myfunction myfunction
help:
	@echo	"make"
	@echo	"make install"
	@echo	"make clean"
	@echo	"make clean"