package main

import (
	"context"

	"github.com/karetskiiVO/robotics/task2/solution/internal/logger"
	"github.com/karetskiiVO/robotics/task2/solution/internal/robot"
	"github.com/phpgao/tlog"
)

func main() {
	tlog.SetLogger(&logger.Logger{})
	tlog.SetLevel("", tlog.LevelDebug)

	r, err := robot.NewRobot(context.TODO())
	if err != nil {
		tlog.Fatal(err)
	}
	defer r.Dispose()

	err = r.Run()
	if err != nil {
		tlog.Error(err)
	}
}
