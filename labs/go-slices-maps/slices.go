package main

import "golang.org/x/tour/pic"

func Pic(dx, dy int) [][]uint8 {

	var arr [][]uint8
	
	arr = make([][]uint8, dy, dy)
	
	for i:=0 ; i<dy ; i++ {
		arr[i] = make([]uint8, dx, dx)
		for j:=0; j<dx; j++{
			arr[i][j] = (uint8(j)*uint8(i))
		}
	}

	return arr
}

func main() {
	pic.Show(Pic)
}
