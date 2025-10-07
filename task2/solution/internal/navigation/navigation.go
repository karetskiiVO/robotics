package navigation

import (
	"github.com/Dobefu/vectors"
	"github.com/karetskiiVO/robotics/task2/solution/internal/telemetry"
)

type Navigator interface {
	Position() vectors.Vector2
	Velocity() vectors.Vector2
	AngularVelocity() vectors.Vector2
	Heading() float64
	Step(*telemetry.Telemetry)
}

type Commander interface {
	MoveTo(vectors.Vector2)
	Run()
}
