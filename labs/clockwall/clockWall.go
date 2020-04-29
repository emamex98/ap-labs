package main

import (
	"fmt"
	"io"
	"net"
	"os"
	"strings"
)

func handleConnC(host string, wait chan int) {

	conn, err := net.Dial("tcp", host)

	if err != nil {
		fmt.Println("Error while stablishing connection!")
		return
	}

	done := make(chan int)

	go func() {
		io.Copy(os.Stdout, conn)
		done <- 2
		wait <- 1
	}()

	close(done)

}

func main() {

	args := os.Args

	if len(args) < 2 {
		fmt.Println("Error - Usage is: go run clockWall.go <TimeZone>=<host>:<port> ...")
		os.Exit(1)
	}

	var hosts []string

	for i := 1; i < len(args); i++ {
		s := strings.Split(args[i], "=")
		hosts = append(hosts, s[1])
	}

	wait := make(chan int)

	for _, host := range hosts {
		go handleConnC(host, wait)
	}
	<-wait

	close(wait)

}
