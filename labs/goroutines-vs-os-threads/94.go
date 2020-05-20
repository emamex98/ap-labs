package main

import (
	"fmt"
	"os"
	"os/signal"
	"runtime"
	"time"
)

func join(current, destiny chan time.Time) {
	for t := range current {
		destiny <- t
	}
}

func handleRoutine(last chan time.Time, nID int, f *os.File) {
	startTime := <-last
	endTime := time.Now()
	timeDiff := endTime.Sub(startTime)

	var m runtime.MemStats
	runtime.ReadMemStats(&m)

	result := fmt.Sprintf("`Id: %d - Time: %v - Mem: %v MiB of %v MiB` \n\n", nID, timeDiff, bToMb(m.Alloc), bToMb(m.TotalAlloc))
	f.WriteString(result)
}

func main() {

	first := make(chan time.Time)
	last := first
	nRoutines := 0

	f, err := os.Create("Routines-Report.md")
	f.WriteString("# Runtime Report \n\n")

	if err != nil {
		fmt.Println(err)
		return
	}

	c := make(chan os.Signal, 1)
	signal.Notify(c, os.Interrupt)
	go func() {
		for sig := range c {
			fmt.Print(sig)
			f.Close()
			os.Exit(0)
		}
	}()

	for {
		nRoutines++
		go handleRoutine(last, nRoutines, f)

		first <- time.Now()
		tmpLast := last
		last = make(chan time.Time)

		go join(tmpLast, last)
	}

	f.Close()

	if err != nil {
		fmt.Println(err)
		return
	}

	fmt.Printf("Report generated.\n")

}

func bToMb(b uint64) uint64 {
	return b / 1024 / 1024
}
