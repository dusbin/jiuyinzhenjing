BIN=myfunction
.PHONY:BIN install uninstall inst uninst clean help
BIN:
	go build .
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
help:
	@echo	"make               #编译"
	@echo	"make clean         #清除"
	@echo	"make inst          #安装"
	@echo	"make install       #编译&安装"
	@echo	"make uninst        #卸载"
	@echo	"make uninstall     #清除&卸载"