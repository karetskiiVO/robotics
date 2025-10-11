package main

import (
	"fmt"
	"math/rand"
	"gonum.org/v1/plot/vg"
	"github.com/karetskiiVO/robotics/task2/solution/internal/numtest"
)

var _ = rand.New
var _ = fmt.Printf

func main() {
	xs := linspace(0, 40, 10)	
	t := numtest.NewLinTester(2, 2, 1)
	linPlt := numtest.PlotGH(t, xs)
	linPlt.Save(30*vg.Centimeter, 20*vg.Centimeter, "test.png")
}

func linspace(start float64, size int, stop float64) []float64 {
	vec := make([]float64, size)
	for i := range vec {
		vec[i] = start
		start += (stop - start) / float64(size)
	}
	return vec
}
