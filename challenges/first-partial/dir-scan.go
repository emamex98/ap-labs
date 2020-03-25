package main

import (
	"fmt"
	"os"
	"path/filepath"
	// "strings"
	//"strconv"
	//"errors"
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
	fmt.Println("Path		", dir)
	fmt.Println("Directories	:", nDir)
	fmt.Println("Symbolic Links	:", nSym)
	fmt.Println("Devices		:", nDev)
	fmt.Println("Sockets		:", nSok)
	fmt.Println("Other files	:", nReg)

	return nil
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}

	scanDir(os.Args[1])
}
