package main

import (
	"fmt"
	"github.com/karetskiiVO/robotics/task2/solution/internal/navfront"
	"github.com/karetskiiVO/robotics/task2/solution/internal/navigation"
	"gonum.org/v1/gonum/spatial/r2"
)

var _ = fmt.Printf
var _ = navigation.CmdLoop

func main() {
	nav := navfront.NewGHNavigator()
	cmd := new(navfront.DumbCommander)
	navigation.NavSetup(nav)
	navigation.CmdSetup(cmd)
	cmd.MoveTo(r2.Vec{X: 2, Y: 2})
	go navigation.NavLoop()
	go cmd.Loop()

	for {
		navigation.CmdLoop()
	}
}
