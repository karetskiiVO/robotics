package navigation

import (
	"github.com/Dobefu/vectors"
	"github.com/karetskiiVO/robotics/task2/solution/internal/telemetry"
)

type DumbNavigator struct {
	pos vectors.Vector2
	vel vectors.Vector2
	omega vectors.Vector2
	heading float64
}

func (nav *DumbNavigator) Step(telem *telemetry.Telemetry) {
	nav.pos.X = float64(telem.Header.OdomX)
	nav.pos.Y = float64(telem.Header.OdomY)

	nav.vel.X = float64(telem.Header.Vx)
	nav.vel.Y = float64(telem.Header.Vy)
	
	nav.omega.X = float64(telem.Header.Wx)
	nav.omega.Y = float64(telem.Header.Wy)
	nav.heading = float64(telem.Header.OdomTh)
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
