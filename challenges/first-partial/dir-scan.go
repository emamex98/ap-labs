package main

import (
	"fmt"
	"os"
	"path/filepath"
	"strconv"
	"github.com/olekukonko/tablewriter"
)

// scanDir stands for the directory scanning implementation
func scanDir(dir string) error {

	var nDir int = -1
	var nSym int = 0
	var nSok int = 0
	var nDev int = 0
	var nReg int = 0

	err := filepath.Walk(dir, func(path string, info os.FileInfo, err error) error {
		
		fileInfo, err := os.Lstat(path)

		if info.IsDir() {
			// Handle directories
			nDir++
		} else if fileInfo.Mode()&os.ModeSymlink == os.ModeSymlink {
			// Handle symbolic links
			nSym++
		} else if filepath.Ext(path) == ".sock" {
			// Handle sockets
			nSok++
		} else if fileInfo.Mode().IsRegular()  {
			// Handle regular files
			nReg++
		} else {
			// Handle devices
			nDev++;
		}

		return nil
	})

	if err != nil {
		panic(err)
	}

	fmt.Println("Directory Scanner Tool")

	data := [][]string{
		[]string{"Directories", strconv.Itoa(nDir)},
		[]string{"Symbolic Links", strconv.Itoa(nSym)},
		[]string{"Devices", strconv.Itoa(nDev)},
		[]string{"Sockets", strconv.Itoa(nSok)},
		[]string{"Other files", strconv.Itoa(nReg)},
	}
	
	table := tablewriter.NewWriter(os.Stdout)
	table.SetHeader([]string{"Path", dir})
	
	for _, v := range data {
		table.Append(v)
	}

	table.SetAlignment(tablewriter.ALIGN_LEFT)
	table.SetAutoFormatHeaders(false)
	table.SetHeaderAlignment(tablewriter.ALIGN_LEFT)
	table.Render()

	return nil
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}

	scanDir(os.Args[1])
}
