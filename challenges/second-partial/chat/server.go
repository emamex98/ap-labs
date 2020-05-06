// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"net"
	"os"
	"regexp"
	"strings"
	"time"
)

type client chan<- string // an outgoing message channel

type user struct {
	id         int
	username   string
	ip_address string
	conn       net.Conn
	ch         chan string
	is_admin   bool
}

var (
	entering  = make(chan client)
	leaving   = make(chan client)
	messages  = make(chan string) // all incoming client messages
	users     = make(map[string]user)
	userCount = 0
	adminID   = -1
)

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

func listUsers() string {
	var response = "irc-server > "

	for key := range users {
		response += key + ", "
	}

	return strings.TrimRight(response, ", ")
}

func sendDirectMessage(sender string, receiver string, message string) {

	if _, ok := users[receiver]; ok {
		usr := users[receiver]
		usr.ch <- sender + " > " + message

	} else {
		usr := users[sender]
		usr.ch <- "irc-server > The username " + receiver + " was not found."
	}
}

func getTime() string {
	t := time.Now()
	zone, _ := t.Zone()
	return "irc-server > Local Time: " + zone + t.Format(" 15:04")
}

func getUserDetails(username string) string {
	var response = "irc-server > "

	if _, ok := users[username]; ok {
		usr := users[username]
		response += "Username: " + usr.username + ", IP Adress: " + usr.ip_address
	} else {
		response += "The username " + username + " was not found."
	}

	return response
}

func kickOut(admin string, outusr string) {
	adm := users[admin]

	if adm.id != adminID {
		adm.ch <- "irc-server > You need to be an admin to kick someone out."
		return
	}

	if _, ok := users[outusr]; ok {
		out := users[outusr]

		if out.id == adminID {
			adminID = -1
			transferAdminRights()
		}

		ch := make(chan string)
		go clientWriter(out.conn, ch)

		ch <- "irc-server > You've been kicked out of this channel \nirc-server > Bad language is not allowed on this channel"

		delete(users, outusr)
		out.conn.Close()

	} else {
		adm.ch <- "irc-server > The username " + outusr + " was not found."
	}
}

func transferAdminRights() {
	for key, item := range users {
		if item.is_admin == false {
			adminID = item.id
			item.is_admin = true
			users[key] = item
			fmt.Println("irc-server > " + item.username + " has been promoted to admin")
			break
		}
	}
}

func handleConn(conn net.Conn, username string) {

	ch := make(chan string)
	go clientWriter(conn, ch)

	if _, ok := users[username]; ok {
		ch <- "Username already exists, try again with a different username."
		conn.Close()

	} else {
		usr := user{userCount, username, conn.RemoteAddr().String(), conn, ch, false}
		users[username] = usr

		userCount++
		fmt.Println("irc-server > New connected user: " + usr.username)

		if adminID == -1 {
			adminID = usr.id
			usr.is_admin = true
			users[username] = usr
			fmt.Println("irc-server > " + usr.username + " has been promoted to admin")
		}

		ch <- "irc-server > You are " + usr.username
		messages <- "irc-server > " + usr.username + " has arrived"
		entering <- ch

		input := bufio.NewScanner(conn)

		for input.Scan() {

			if match, _ := regexp.MatchString("/users", input.Text()); match {
				ch <- listUsers()

			} else if match, _ := regexp.MatchString("/msg [0-9A-Za-z_ ]+", input.Text()); match {
				inp := strings.Split(input.Text(), " ")
				var trim = "/msg " + inp[1]
				message := strings.TrimLeft(input.Text(), trim)
				sendDirectMessage(usr.username, inp[1], message)

			} else if match, _ := regexp.MatchString("/time", input.Text()); match {
				ch <- getTime()

			} else if match, _ := regexp.MatchString("/user [0-9A-Za-z_]+", input.Text()); match {
				usrname := strings.Split(input.Text(), " ")
				ch <- getUserDetails(usrname[1])

			} else if match, _ := regexp.MatchString("/kick [0-9A-Za-z_]+", input.Text()); match {
				inp := strings.Split(input.Text(), " ")
				kickOut(usr.username, inp[1])

			} else {
				messages <- usr.username + " > " + input.Text()
			}
		}

		leaving <- ch

		if _, ok := users[username]; !ok {
			messages <- "irc-server > " + usr.username + " has been kicked out."
			fmt.Println("irc-server > " + usr.username + " has been kicked out.")

		} else {
			messages <- "irc-server > " + usr.username + " has left."
			fmt.Println("irc-server > " + usr.username + " has left.")

			delete(users, username)

			if usr.id == adminID {
				adminID = -1
				transferAdminRights()
			}

			conn.Close()

		}
	}
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!+main
func main() {

	// Handle flags
	fhost := flag.String("host", "", "host")
	fport := flag.String("port", "", "port")

	flag.Parse()

	if *fhost == "" || *fport == "" {
		fmt.Printf("Missing arguments. Usage: go run server.go -host <host> -port <port>")
		os.Exit(1)
	}

	listener, err := net.Listen("tcp", *fhost+":"+*fport)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println("irc-server > Simple IRC Server started at " + *fhost + ":" + *fport)
	fmt.Println("irc-server > Ready to receive new clients")

	go broadcaster()

	for {
		conn, err := listener.Accept()

		if err != nil {
			log.Print(err)
			continue
		}

		usr, _ := bufio.NewReader(conn).ReadString('$')
		username := strings.TrimRight(usr, "$")

		go handleConn(conn, username)

	}
}

//!-main
