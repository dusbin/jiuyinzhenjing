#! /bin/bash
gcc -o websocket_server main.c tcp.c tcp.h -lssl -lcrypto
