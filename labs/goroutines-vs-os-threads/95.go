package main

import (
	"fmt"
	"os"
	"strconv"
	"time"
)

func pingPong(count int) {

	ping := make(chan int)
	pong := make(chan int)
	done := make(chan struct{})

	go func() {
		for n := 0; n < count; n++ {
			ping <- n
			<-pong
		}
		close(ping)
		close(done)
	}()

	go func() {
		for n := range ping {
			pong <- n
		}
		close(pong)
	}()

	<-done

}

func main() {

	//pingCount := 0

	if len(os.Args) < 2 {
		fmt.Println("Error - Usage: go run 95.go <number-of-ping-pongs>")
		os.Exit(1)
	}

	pingCount, err := strconv.Atoi(os.Args[1])
	startTime := time.Now()

	pingPong(pingCount)

	endTime := time.Now()
	timeDiff := endTime.Sub(startTime)

	elapsedSec := float64(timeDiff.Nanoseconds()) / 1000000000.0
	perSec := float64(pingCount) / elapsedSec

	f, err := os.Create("PingPong-Report.md")

	if err != nil {
		fmt.Println(err)
		return
	}

	result := fmt.Sprintf("# Performance Report \n\n **Time elapsed:** %v \n\n**Ping-pongs:** %v \n\n**PP/second:** %f", timeDiff, pingCount, perSec)
	f.WriteString(result)
	f.Close()

	if err != nil {
		fmt.Println(err)
		return
	}

	fmt.Printf("Report generated.\n")
}
