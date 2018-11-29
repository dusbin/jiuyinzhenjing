BIN=main
.PHONY:bin clean help run osinfo userinfo langinfo svgdemo
help:
	@echo	"make run           #run"
	@echo	"make bin           #bin"
	@echo	"make osinfo        #osinfo"
	@echo	"make userinfo      #userinfo"
	@echo	"make langinfo      #langinfo"
	@echo	"make svgdemo       #svgdemo"
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
clean:
	rm -rf main osinfo.so userinfo.so langinfo.so svgdemo.so
