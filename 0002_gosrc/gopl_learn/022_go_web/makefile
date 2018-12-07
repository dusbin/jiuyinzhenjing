BIN=main
all:bin osinfo userinfo langinfo svgdemo wsclient wsserver
.PHONY:all bin clean help run osinfo userinfo langinfo svgdemo wsclient wsserver
help:
	@echo	"make all           #all"
	@echo	"make run           #run"
	@echo	"make bin           #bin"
	@echo	"make osinfo        #osinfo"
	@echo	"make userinfo      #userinfo"
	@echo	"make langinfo      #langinfo"
	@echo	"make svgdemo       #svgdemo"
	@echo	"make wsclient      #wsclient"
	@echo	"make wsserver      #wsserver"
	@echo   "make clean         #清除"
run:
	go run main.go
bin:
	go build main.go
osinfo:
	go build -tags osinfo -buildmode=plugin -o osinfo.so osinfo/osinfo.go
userinfo:
	go build -tags userinfo -buildmode=plugin -o userinfo.so userinfo/userinfo.go
langinfo:
	go build -tags langinfo -buildmode=plugin -o langinfo.so langinfo/langinfo.go
svgdemo:
	go build -tags svgdemo -buildmode=plugin -o svgdemo.so svgdemo/svgdemo.go
wsclient:
	go build -tags wsclient -buildmode=plugin -o wsclient.so wsclient/wsclient.go
wsserver:
	go build -tags wsserver -buildmode=plugin -o wsserver.so wsserver/wsserver.go
clean:
	rm -rf main osinfo.so userinfo.so langinfo.so svgdemo.so wsclient.so wsserver.so
