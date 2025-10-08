package main

import (
	"fmt"
	"github.com/Dobefu/vectors"
	"github.com/karetskiiVO/robotics/task2/solution/internal/navfront"
	"github.com/karetskiiVO/robotics/task2/solution/internal/navigation"
)

var _ = fmt.Printf
var _ = navigation.CmdLoop

func main() {
	nav := new(navfront.DumbNavigator)
	cmd := new(navfront.DumbCommander)
	navigation.NavSetup(nav)
	navigation.CmdSetup(cmd)
	cmd.MoveTo(vectors.Vector2{X: 0.5, Y: 0.5})

	for {
		navigation.NavLoop()
		navigation.CmdLoop()
	}
}
