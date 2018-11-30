BIN=main
.PHONY:bin plugin1 plugin2 clean help
help:
	@echo	"make bin           #bin"
	@echo	"make plugin1       #plugin1"
	@echo	"make plugin2       #plugin2"
	@echo   "make clean         #清除"
bin:
	go build main.go
plugin1:
	go build -tags plugin1 -buildmode=plugin -o plugin1.so plugin1.go
plugin2:
	go build -tags plugin2 -buildmode=plugin -o plugin2.so plugin2.go
clean:
	rm -rf main plugin1.so plugin2.so
