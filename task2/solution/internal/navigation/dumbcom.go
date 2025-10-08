package navigation

import (
	"math"
	"github.com/Dobefu/vectors"
	//"github.com/karetskiiVO/robotics/task2/solution/internal/marshall"	
)

type Maneuver int
const (
	_ = iota
	maneuverDone Maneuver = 0
	maneuverTurning Maneuver = 1
	maneuverLinear Maneuver = 2
)

type DumbCommander struct {
	Nav Navigator
	con movement.Controller
	state Maneuver
	targetPosition vectors.Vector2
	targetHeading float64
}

const angleTolerance float64 = 1e-2
const distanceTolerance float64 = 1e-2
const velocity float64 = 0.5
const maxLinearAcc float64 = 0.05

func (com *DumbCommander) Init(address string) {
	com.con.Connect(address)
	com.Nav = new(DumbNavigator);
}

func (com *DumbCommander) MoveTo(point vectors.Vector2) {
	currentPos := com.Nav.Position()
	com.targetPosition = point

	point.Sub(currentPos)
	com.targetHeading = point.AngleRadians()

	com.state = maneuverTurning
}

func (com *DumbCommander) Run() {
	switch com.state {
		case maneuverTurning: {
			if withinTolerance(com.Nav.Heading(), com.targetHeading, angleTolerance) {
				com.state = maneuverLinear
				com.con.SetState(float32(velocity), 0)
			}
		}
		case maneuverLinear: {
			predPos := predictPosition(com.Nav.Position(), com.Nav.Velocity())
			dist := distance(predPos, com.targetPosition)
			if withinTolerance(dist, 0, distanceTolerance) {
				com.state = maneuverDone
				com.con.SetState(0, 0)
			}
		}
		case maneuverDone:
			return
	}
}

func withinTolerance(val float64, target float64, tol float64) bool {
	return math.Abs(target - val) / target < tol
}

func distance(x vectors.Vector2, y vectors.Vector2) float64 {
	y.Sub(x)
	return y.Magnitude()
}

// Predicts position after deceleration from a given velocity, assuming 
// that robot will decelerate with 0 angular velocity
func predictPosition(pos vectors.Vector2, vel vectors.Vector2) vectors.Vector2 {
	displMagn := math.Pow(vel.Magnitude(), 2) / (2*maxLinearAcc)
	deltaR := vectors.Vector2{
		X: pos.X - displMagn,
		Y: pos.Y - displMagn,
	}
	return deltaR
}
