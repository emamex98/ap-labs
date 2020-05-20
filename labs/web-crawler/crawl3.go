// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 241.

// Crawl2 crawls web links starting with the command-line arguments.
//
// This version uses a buffered channel as a counting semaphore
// to limit the number of concurrent calls to links.Extract.
//
// Crawl3 adds support for depth limiting.
//
package main

import (
	"flag"
	"fmt"
	"log"

	"github.com/todostreaming/gopl.io/ch5/links"
)

//!+sema
// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
var tokens = make(chan struct{}, 20)
var passed = make(map[string]bool)
var totalD int

func crawl(url string) []string {
	fmt.Println(url)
	tokens <- struct{}{} // acquire a token
	list, err := links.Extract(url)
	<-tokens // release the token

	if err != nil {
		log.Print(err)
	}
	return list
}

//!-sema

func preCrawl(url string, depth int, done chan<- bool) {

	defer func() {
		done <- true
	}()

	if depth <= 0 {
		return
	}

	_, ok := passed[url]

	if ok {
		return
	} else {
		fmt.Print("Depth Level ", (totalD-depth)+1, ": ")
		passed[url] = true
	}

	list := crawl(url)

	for _, link := range list {
		isRecursive := make(chan bool)
		go preCrawl(link, depth-1, isRecursive)
		<-isRecursive
	}

}

//!+
func main() {

	depth := flag.Int("depth", 1, "depth")
	flag.Parse()

	totalD = *depth

	worklist := make(chan []string)
	done := make(chan bool)

	// Start with the command-line arguments.
	go func() { worklist <- flag.Args() }()

	list := <-worklist

	go preCrawl(list[0], *depth, done)
	<-done

	// Crawl the web concurrently.
	// seen := make(map[string]bool)
	// for ; n > 0; n-- {
	// 	list := <-worklist
	// 	for _, link := range list {
	// 		if !seen[link] {
	// 			seen[link] = true
	// 			n++
	// 			go func(link string) {
	// 				worklist <- crawl(link)
	// 			}(link)
	// 		}
	// 	}
	// }
}

//!-
