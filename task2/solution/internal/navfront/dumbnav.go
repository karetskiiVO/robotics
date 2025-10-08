package navfront

import (
	"math"
	"github.com/Dobefu/vectors"
	"github.com/karetskiiVO/robotics/task2/solution/internal/marshall"
)

type DumbNavigator struct {
	pos vectors.Vector2
	vel vectors.Vector2
	omega vectors.Vector2
	heading float64
}

func (nav *DumbNavigator) Step(telem *marshall.TelemPacket) {
	nav.pos.X = float64(telem.Header.OdomX)
	nav.pos.Y = float64(telem.Header.OdomY)

	nav.omega.X = float64(telem.Header.Wx)
	nav.omega.Y = float64(telem.Header.Wy)
	nav.heading = float64(telem.Header.OdomTh)

	nav.vel.X = float64(telem.Header.V) * math.Sin(nav.Heading())
	nav.vel.Y = float64(telem.Header.V) * math.Cos(nav.Heading())
}

func (nav *DumbNavigator) Position() vectors.Vector2 {
	return nav.pos
}

func (nav *DumbNavigator) Velocity() vectors.Vector2 {
	return nav.vel
}

func (nav *DumbNavigator) AngularVelocity() vectors.Vector2 {
	return nav.omega
}

func (nav *DumbNavigator) Heading() float64 {
	return nav.heading
}
