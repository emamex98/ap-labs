package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
	"time"
)

type Point struct{ x, y float64 }

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

func RandFloat() float64 {
	rand.Seed(time.Now().UnixNano())
	var r = -100.0 + rand.Float64()*(200)

	return math.Floor(r*10) / 10
}

func X(point Point) float64 {
	return point.x
}
func Y(point Point) float64 {
	return point.y
}

func OnSegment(p, q, r Point) bool {
	if (X(q) <= math.Max(X(p), X(r))) && (X(q) >= math.Min(X(p), X(r))) && (Y(q) <= math.Max(Y(p), Y(r))) && (Y(q) >= math.Min(Y(p), Y(r))) {
		return true
	}

	return false
}

func GetOrientation(p, q, r Point) int {

	var val = ((Y(q) - Y(p)) * (X(r) - X(q))) - ((X(q) - X(p)) * (Y(r) - Y(q)))

	if val > 0 {
		// Clockwise orientation
		return 1
	} else if val < 0 {
		// Counterclockwise orientation
		return 2
	} else {
		// Colinear orientation
		return 0
	}

}

func DoIntersect(p1, q1, p2, q2 Point) bool {

	var o1 = GetOrientation(p1, q1, p2)
	var o2 = GetOrientation(p1, q1, q2)
	var o3 = GetOrientation(p2, q2, p1)
	var o4 = GetOrientation(p2, q2, q1)

	if (o1 != o2) && (o3 != o4) {
		// General case
		return true
	}

	// Special Cases:

	if (o1 == 0) && OnSegment(p1, p2, q1) {
		// p1 , q1 and p2 are colinear and p2 lies on segment p1q1
		return true
	}

	if (o2 == 0) && OnSegment(p1, q2, q1) {
		// p1 , q1 and q2 are colinear and q2 lies on segment p1q1
		return true
	}

	if (o3 == 0) && OnSegment(p2, p1, q2) {
		// p2 , q2 and p1 are colinear and p1 lies on segment p2q2
		return true
	}

	if (o4 == 0) && OnSegment(p2, q1, q2) {
		// p2 , q2 and q1 are colinear and q1 lies on segment p2q2
		fmt.Println("Caso 5")
		return true
	}

	return false

}

func Perimeter(sides int) int {

	fmt.Printf("- Generating a [%v] sides figure\n", sides)

	var points []Point
	var edges []float64
	var edge = 0.0
	var i = 1
	var inv = false

	p := Point{RandFloat(), RandFloat()}
	points = append(points, p)

	for len(edges) < (sides - 1) {

		p := Point{RandFloat(), RandFloat()}
		points = append(points, p)

		if len(edges) == 0 {

			edge = Distance(points[i-1], points[i])
			edges = append(edges, math.Floor(edge*10)/10)

		} else {

			for j := i - 2; j > 0; j-- {

				inv = false
				inv = DoIntersect(points[i], points[i-1], points[j], points[j-1])

				if inv == true {

					points = points[:len(points)-1]
					p := Point{RandFloat(), RandFloat()}
					points = append(points, p)

					j++
				}
			}

			edge = Distance(points[i-1], points[i])
			edges = append(edges, math.Floor(edge*10)/10)

		}

		i++
	}

	edge = Distance(points[i-1], points[0])
	edges = append(edges, math.Floor(edge*10)/10)

	fmt.Println("- Figure's vertices")

	for i := 0; i < len(points); i++ {
		fmt.Printf("  - (%v, %v)\n", X(points[i]), Y(points[i]))
	}

	fmt.Println("- Figure's Perimeter")
	fmt.Printf("  - ")

	var perimeter = 0.0

	for i := 0; i < len(edges)-1; i++ {
		perimeter += edges[i]
		fmt.Printf(" %v +", edges[i])
	}

	perimeter += edges[i-1]
	fmt.Printf(" %v = %v\n", edges[i-1], math.Floor(perimeter*10)/10)

	return 0
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Error: a number of sides is needed ad an argument.")
		os.Exit(1)
	}

	sides, err := strconv.Atoi(os.Args[1])

	if sides < 3 {
		fmt.Println("Al least 3 sides are needed to a polygon.")
		os.Exit(1)
	}

	if err == nil {
		Perimeter(sides)
	}

}
