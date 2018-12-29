package main

import (
	"bytes"
	"fmt"
	"github.com/jordan-wright/email"
	"html/template"
	"net/smtp"
	"time"
)

func main() {
	fromUser := "duzhengbin<847379962@qq.com>"
	toUser := "duzb@uxsino.com"
	subject := "hello,world"
	err := SendMail(fromUser, toUser, subject)
	if err != nil {
		fmt.Println("Send failed")
	} else {
		fmt.Println("Send Success")
	}
}
func SendMail(fromUser, toUser, subject string) error {
	// NewEmail 返回一个email结构体的指针
	e := email.NewEmail()
	// 发件人
	e.From = fromUser
	// 收件人（可以是多个）
	e.To = []string{toUser}
	// 邮件主题
	e.Subject = subject
	// 解析html模板
	t, err := template.ParseFiles("email-template.html")
	if err != nil {
		return err
	}
	// Buffer是一个实现了读写方法的可变大小字节缓冲
	body := new(bytes.Buffer)
	// Execute 方法将解析好的模板应用到匿名结构体上，并将输出写入body中
	t.Execute(body, struct {
		FromUserName string
		ToUserName   string
		TimeDate     string
		Message      string
	}{
		FromUserName: "go语言",
		ToUserName:   "Sixah",
		TimeDate:     time.Now().Format("2018/01/02 00:00"),
		Message:      "golang是世界上最好的语言",
	})
	// HTML形式的消息
	e.HTML = body.Bytes()
	// 从缓冲中将内容作为附件到邮件中
	e.Attach(body, "email-template.html", "text/html")
	// 以路径将文件作为附件添加到邮件中
	e.AttachFile("./example.go")
	// 发送邮件(如果使用QQ邮箱发送邮件的话，passwd不是邮箱密码而是授权码)
	return e.Send("smtp.qq.com:587", smtp.PlainAuth("", "847379962@qq.com", "passwd", "smtp.qq.com"))
}

// https://studygolang.com/articles/17287
