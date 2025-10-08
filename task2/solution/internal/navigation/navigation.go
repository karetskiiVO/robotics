package navigation

import (
	"fmt"
	"os"
	"sync"
	"log"
	"github.com/Dobefu/vectors"
	"github.com/karetskiiVO/robotics/task2/solution/internal/marshall"
)

var _ = fmt.Printf

type Navigator interface {
	Position() vectors.Vector2
	Velocity() vectors.Vector2
	AngularVelocity() vectors.Vector2
	Heading() float64
	Step(*marshall.TelemPacket)
}

type Commander interface {
	MoveTo(vectors.Vector2)
	Step() (float64, float64)
}

// Временное решение, чтобы скрыть реализацию бэкенда

var telemHandle *marshall.Telemetry = nil
var ctrHandle *marshall.Commands = nil

var navOnce sync.Once
var cmdOnce sync.Once

var navInstance Navigator
var cmdInstance Commander

func NavSetup(nav Navigator) {
	navOnce.Do(func() {
		telemPort := loadParam("TELEMETRY_PORT", "5600")
		telemHost := loadParam("TELEMETRY_HOST", "127.0.0.1")

		telemHandle = new(marshall.Telemetry)
		err := telemHandle.Setup(telemHost+":"+telemPort)
		if err != nil {
			log.Fatal(err)
		}

		go telemHandle.Loop()
		log.Println("Telemetry link up")
		navInstance = nav
	})
}

func NavLoop() {
	telemData := telemHandle.Receive()
	navInstance.Step(telemData)
}

func CmdSetup(cmd Commander) {
	cmdOnce.Do(func() {
		cmdPort := loadParam("CMD_LISTEN_PORT", "5555")
		cmdHost := loadParam("CMD_LISTEN_HOST", "127.0.0.1")

		ctrHandle = new(marshall.Commands)
		err := ctrHandle.Setup(cmdHost+":"+cmdPort)
		if err != nil {
			log.Fatal(err)
		}

		go ctrHandle.Loop()
		log.Println("Command link up")
		cmdInstance = cmd
	})
}

func CmdLoop() {
	v, omega := cmdInstance.Step()
	ctrHandle.SetVelocity(v, omega)
}

func NavInstance() Navigator {
	return navInstance
}

func CmdInstance() Commander {
	return cmdInstance
}

func loadParam(name string, def string) string {
	value, exists := os.LookupEnv(name)
	if !exists {
		return def
	}
	return value
}
