# 创建HTTPS的证书
## 方法一
    OpenSSL方式，生成私钥key.pem和证书cert.pem，3650代表有效期为10年

`openssl genrsa -out key.pem 2048`

`openssl req -new -x509 -key key.pem -out cert.pem -days 3650`
## 方法二
    Golang标准库crypto/tls里有generate_cert.go，可以生成私钥key.pem和证书cert.pem，host参数是必须的，需要注意的是默认有效期是1年 

`go run $GOROOT/src/crypto/tls/generate_cert.go --host localhost`