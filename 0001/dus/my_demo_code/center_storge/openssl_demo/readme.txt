
#编译代码
gcc -Wall ssl_client.c -o client -lssl -lcrypto
gcc -Wall ssl_server.c -o server -lssl -lcrypto
#生成证书私钥
openssl genrsa -out privkey.pem 2048
openssl req -new -x509 -key privkey.pem -out cacert.pem -days 1095
#运行服务端客户端
./server 7838 1 127.0.0.1 cacert.pem privkey.pem
./client 127.0.0.1 7838
#参考资料
#https://www.cnblogs.com/treecarrybear/p/6219769.html