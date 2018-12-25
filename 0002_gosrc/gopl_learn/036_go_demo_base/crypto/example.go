package main

import (
	"crypto/md5"
	"encoding/base64"
	"encoding/hex"
	"fmt"
)

func main() {
	test_md5()
	test_base64()
}
func test_md5() {
	h := md5.New()
	h.Write([]byte("123456"))
	cipherStr := h.Sum(nil)
	fmt.Println(cipherStr)
	fmt.Println(hex.EncodeToString(cipherStr))
}

const (
	base64Table = "123QRSTUabcdVWXYZHijKLAWDCABDstEFGuvwxyzGHIJklmnopqr234560178912"
)

var coder = base64.NewEncoding(base64Table)

func base64Encode(src []byte) []byte {
	return []byte(coder.EncodeToString(src))
}
func base64Decode(src []byte) ([]byte, error) {
	return coder.DecodeString(string(src))
}
func test_base64() {
	// encode
	hello := "hello world"
	debyte := base64Encode([]byte(hello))
	// decode
	fmt.Println(string(debyte))
	enbyte, err := base64Decode(debyte)
	if err != nil {
		fmt.Println(err.Error())
	}
	if hello != string(enbyte) {
		fmt.Println("hello is not equal to enbyte")
	}
	fmt.Println(string(enbyte))
}
