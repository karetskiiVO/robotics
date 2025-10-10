package main

import (
	"fmt"
	"gonum.org/v1/gonum/mat"
	"github.com/karetskiiVO/robotics/task2/solution/internal/navfront"
	"github.com/karetskiiVO/robotics/task2/solution/internal/navigation"
)

var _ = fmt.Printf
var _ = navigation.CmdLoop

func main() {
	nav := navfront.NewDumbNavigator()
	cmd := navfront.NewDumbCommander()
	navigation.NavSetup(nav)
	navigation.CmdSetup(cmd)
	cmd.MoveTo(mat.NewVecDense(2, []float64{2, 2}))
	go navigation.NavLoop()
	go cmd.Loop()

	for {
		navigation.CmdLoop()
	}
}
