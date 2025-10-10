package navfront

import (
	"gonum.org/v1/gonum/mat"
	"github.com/karetskiiVO/robotics/task2/solution/internal/marshall"
	"math"
)

type DumbNavigator struct {
	pos     *mat.VecDense
	vel     *mat.VecDense
	omega   float64
	heading float64
}

func NewDumbNavigator() *DumbNavigator {
	nav := new(DumbNavigator)
	nav.pos = mat.NewVecDense(2, nil)
	nav.vel = mat.NewVecDense(2, nil)
	return nav
}

func (nav *DumbNavigator) Step(telem *marshall.TelemPacket) {
	nav.pos.SetVec(0, float64(telem.Header.OdomX))
	nav.pos.SetVec(1, float64(telem.Header.OdomY))

	nav.omega = float64(telem.Header.Wy)
	nav.heading = float64(telem.Header.OdomTh)

	nav.vel.SetVec(0, float64(telem.Header.V) * math.Sin(nav.Heading()))
	nav.vel.SetVec(1, float64(telem.Header.V) * math.Cos(nav.Heading()))
}

func (nav *DumbNavigator) Position() *mat.VecDense {
	return nav.pos
}

func (nav *DumbNavigator) Velocity() *mat.VecDense {
	return nav.vel
}

func (nav *DumbNavigator) AngularVelocity() float64 {
	return nav.omega
}

func (nav *DumbNavigator) Heading() float64 {
	return nav.heading
}
