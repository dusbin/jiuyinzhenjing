BIN=myfunction
.PHONY:BIN install uninstall inst uninst clean help debug
BIN:
	go build .
debug:
	go build -gcflags "-N -l" -o main main.go
inst:
	cp -f myfunction /bin/
	ln -s /bin/myfunction /bin/myclient
	ln -s /bin/myfunction /bin/myserver
install:
	go build .
	cp -f myfunction /bin/
	ln -s /bin/myfunction /bin/myclient
	ln -s /bin/myfunction /bin/myserver
uninst:
	rm -rf /bin/myclient /bin/myserver /bin/myfunction
uninstall:
	rm -rf /bin/myclient /bin/myserver /bin/myfunction myfunction
clean:
	rm -rf myfunction
test:
	@echo	"\033[32mTest myswitch \033[0m"
	go test ./myserver/myswitch/
help:
	@echo	"make               #编译"
	@echo	"make clean         #清除"
	@echo   "make debug         #编译调试版"
	@echo	"make inst          #安装"
	@echo	"make install       #编译&安装"
	@echo	"make uninst        #卸载"
	@echo	"make uninstall     #清除&卸载"
	@echo	"make test          #测试模块"