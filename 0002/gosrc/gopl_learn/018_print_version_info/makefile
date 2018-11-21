BIN=main
BUILD_TIME		:=$(shell date "+%F %T")
COMMIT_SHA1		:=$(shell git rev-parse HEAD || echo "GitNotFound")
PATH			:=$(shell pwd)
.PHONY:BIN clean help
BIN:
	go build \
 -ldflags " \
 -X '${PATH}/version.BuildTime=${BUILD_TIME}' \
 -X '${PATH}/version.CommitID=${COMMIT_SHA1}' \
 " -o $(BIN) 
clean:
	rm -rf main
help:
	@echo	"make               #编译"
	@echo	"make clean         #清除"