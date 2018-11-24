BIN=main
.PHONY:bin clean help run osinfo userinfo
help:
	@echo	"make run           #run"
	@echo	"make bin           #bin"
	@echo	"make osinfo        #osinfo"
	@echo	"make userinfo      #userinfo"
	@echo   "make clean         #清除"
run:
	go run main.go
bin:
	go build main main.go
osinfo:
	go build -tags osinfo -buildmode=plugin -o osinfo.so osinfo/osinfo.go
userinfo:
	go build -tags userinfo -buildmode=plugin -o userinfo.so userinfo/userinfo.go
clean:
	rm -rf main osinfo.so userinfo.so
