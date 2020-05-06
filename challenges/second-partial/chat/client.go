// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 227.

// Netcat is a simple read/write client for TCP servers.
package main

import (
	"flag"
	"fmt"
	"io"
	"log"
	"net"
	"os"
)

//!+
func main() {

	// Handle flags
	username := flag.String("user", "", "username")
	server := flag.String("server", "", "host:port")

	flag.Parse()

	if *username == "" || *server == "" {
		fmt.Printf("Missing arguments. Usage: go run client.go -user <username> -server <host>:<port>")
		return
	}

	conn, err := net.Dial("tcp", *server)

	if err != nil {
		log.Fatal(err)
	}

	// Pass username to server
	fmt.Fprintf(conn, *username+"$")

	done := make(chan struct{})

	go func() {
		io.Copy(os.Stdout, conn) // NOTE: ignoring errors
		log.Println("** Exited chat. **")
		os.Exit(1)
	}()

	mustCopy(conn, os.Stdin)
	conn.Close()
	<-done // wait for background goroutine to finish
}

//!-

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}
