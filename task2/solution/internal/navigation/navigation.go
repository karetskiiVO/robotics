package navigation

import (
	"fmt"
	"os"
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
	Step(Navigator) (float64, float64)
}

// Временное решение, чтобы скрыть реализацию бэкенда

var telemHandle *marshall.Telemetry = nil
var ctrHandle *marshall.Commands = nil
var telemInited bool = false
var cmdInited bool = false

func NavSetup() {
	if telemInited {
		panic("Nav system if a singleton!")
	}

	telemPort := loadParam("TELEMETRY_PORT", "5600")
	telemHost := loadParam("TELEMETRY_HOST", "127.0.0.1")

	telemHandle = new(marshall.Telemetry)
	telemHandle.Setup(telemHost+":"+telemPort)
	go telemHandle.Loop()

	telemInited = true
}

func NavLoop(nav Navigator) {
	telemData := telemHandle.Receive()
	nav.Step(telemData)
}

func CmdSetup() {
	if telemInited {
		panic("Commander should be a singleton!")
	}
	cmdPort := loadParam("CMD_LISTEN_PORT", "5555")
	cmdHost := loadParam("CMD_LISTEN_HOST", "127.0.0.1")

	ctrHandle = new(marshall.Commands)
	ctrHandle.Setup(cmdHost+":"+cmdPort)
	go ctrHandle.Loop()

	cmdInited = true
}

func CmdLoop(cmd Commander, nav Navigator) {
	v, omega := cmd.Step(nav)
	ctrHandle.SetVelocity(v, omega)
}

func loadParam(name string, def string) string {
	value, exists := os.LookupEnv(name)
	if !exists {
		return def
	}
	return value
}
