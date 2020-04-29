// Clock2 is a concurrent TCP server that periodically writes the time.
package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"time"
)

func handleConn(c net.Conn, tz string) {
	defer c.Close()
	for {
		loc, _ := time.LoadLocation(tz)
		_, err := io.WriteString(c, time.Now().In(loc).Format(tz+" - 15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {

	if len(os.Args) < 3 {
		fmt.Println("Error - Usage is: TZ=<timezone> go run clock2.go -port <port>")
		os.Exit(1)
	}

	timeZone := os.Getenv("TZ")
	portNum := os.Args[2]

	listener, err := net.Listen("tcp", "localhost:"+portNum)

	if err != nil {
		log.Fatal(err)
	}

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn, timeZone) // handle connections concurrently
	}

}
