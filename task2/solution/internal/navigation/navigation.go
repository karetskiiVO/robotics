package navigation

import (
	"fmt"
	"github.com/karetskiiVO/robotics/task2/solution/internal/marshall"
	"gonum.org/v1/gonum/mat"
	"log"
	"os"
	"sync"
)

var _ = fmt.Printf

type Navigator interface {
	Position() *mat.VecDense
	Velocity() *mat.VecDense
	AngularVelocity() float64
	Heading() float64
	Step(*marshall.TelemPacket)
}

type Commander interface {
	MoveTo(*mat.VecDense)
	Step() (float64, float64)
}

type Predict interface {
	Step(*marshall.TelemPacket)
	PredictPosition(float64) *mat.VecDense
	PredictVelocity(float64) *mat.VecDense
	PredictHeading(float64) float64
	PredictAngularVelocity(float64) float64
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
		err := telemHandle.Setup(telemHost + ":" + telemPort)
		if err != nil {
			log.Fatal(err)
		}

		go telemHandle.Loop()
		log.Println("Telemetry link up")
		navInstance = nav
	})
}

func NavLoop() {
	for {
		telemData := telemHandle.Receive()
		/*
			Неправильный разделитель в принятых данных мы обработали раньше
			(в telem.Loop()), так что если магия нулевая, то это метка для интерполяции
		*/
		if telemData.Header.Magic == 0 {
			log.Println("Telem timeout")
		}
		navInstance.Step(telemData)
	}
}

func CmdSetup(cmd Commander) {
	cmdOnce.Do(func() {
		cmdPort := loadParam("CMD_LISTEN_PORT", "5555")
		cmdHost := loadParam("CMD_LISTEN_HOST", "127.0.0.1")

		ctrHandle = new(marshall.Commands)
		err := ctrHandle.Setup(cmdHost + ":" + cmdPort)
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
