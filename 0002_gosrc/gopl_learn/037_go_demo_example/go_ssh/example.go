package main

import (
	"golang.org/x/crypto/ssh"
	"log"
	"os"
)

func main() {
	ce := func(err error, msg string) {
		if err != nil {
			log.Fatalf("%s error: %v", msg, err)
		}
	}
	client, err := ssh.Dial("tcp", "127.0.0.1:22", &ssh.ClientConfig{
		User:            "root",
		Auth:            []ssh.AuthMethod{ssh.Password("123456")},
		HostKeyCallback: ssh.InsecureIgnoreHostKey(),
	})
	ce(err, "dial")
	session, err := client.NewSession()
	ce(err, "new session")
	defer session.Close()
	session.Stdout = os.Stdout
	session.Stderr = os.Stderr
	session.Stdin = os.Stdin
	modes := ssh.TerminalModes{
		ssh.ECHO:          0,
		ssh.TTY_OP_ISPEED: 14400,
		ssh.TTY_OP_OSPEED: 14400,
	}
	err = session.RequestPty("[linux](https://www.linuxprobe.com/linux)", 32, 160, modes)
	ce(err, "request pty")
	err = session.Shell()
	ce(err, "start shell")
	err = session.Wait()
	ce(err, "return")
}
