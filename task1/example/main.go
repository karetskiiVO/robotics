package main

import (
	"context"
	"log"

	"github.com/karetskiiVO/robotics/task1/example/internal/robot"
)

func main() {
	r, err := robot.NewRobot(context.TODO())
	if err != nil {
		log.Fatal(err)
	}
	defer r.Dispose()

	err = r.Run()
	if err != nil {
		log.Fatal(err)
		return
	}
}
