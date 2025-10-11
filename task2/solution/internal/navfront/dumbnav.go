package navfront

import (
	"gonum.org/v1/gonum/spatial/r2"
	"github.com/karetskiiVO/robotics/task2/solution/internal/marshall"
	"math"
)

type DumbNavigator struct {
	pos     r2.Vec
	vel     r2.Vec
	omega   float64
	heading float64
}

func (nav *DumbNavigator) Step(telem *marshall.TelemPacket) {
	nav.pos.X = float64(telem.Header.OdomX)
	nav.pos.Y = float64(telem.Header.OdomY)

	nav.omega = float64(telem.Header.Wy)
	nav.heading = float64(telem.Header.OdomTh)

	nav.vel.X = float64(telem.Header.V) * math.Sin(nav.Heading())
	nav.vel.Y = float64(telem.Header.V) * math.Cos(nav.Heading())
}

func (nav *DumbNavigator) Position() r2.Vec {
	return nav.pos
}

func (nav *DumbNavigator) Velocity() r2.Vec {
	return nav.vel
}

func (nav *DumbNavigator) AngularVelocity() float64 {
	return nav.omega
}

func (nav *DumbNavigator) Heading() float64 {
	return nav.heading
}
