#! /bin/bash
gcc -o client client.c -g
gcc -o server server.c -g -lcrypt
