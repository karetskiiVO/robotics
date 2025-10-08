package navfront

import (
	"math"
	"log"
	"github.com/Dobefu/vectors"
	"github.com/karetskiiVO/robotics/task2/solution/internal/navigation"	
)

type Maneuver int
const (
	_ = iota
	maneuverDone Maneuver = 0
	maneuverTurning Maneuver = 1
	maneuverLinear Maneuver = 2
)

type DumbCommander struct {
	state Maneuver
	targetPosition vectors.Vector2
	targetHeading float64
}

const angleTolerance float64 = 1e-2
const distanceTolerance float64 = 1e-2
const maxVelocity float64 = 0.5
const maxOmega float64 = 1.0
const maxLinearAcc float64 = 0.05

func (com *DumbCommander) MoveTo(point vectors.Vector2) {
	nav := navigation.NavInstance()
	currentPos := nav.Position()
	com.targetPosition = point

	point.Sub(currentPos)
	com.targetHeading = point.AngleRadians()
	log.Printf("TargHeading: %f deg, deltaR: %v {m, m}\n", radToDeg(com.targetHeading), point)

	com.state = maneuverTurning
	log.Println("Started maneuver: turning")
}

func (com *DumbCommander) Step() (float64, float64) {
	nav := navigation.NavInstance()
	switch com.state {
		case maneuverTurning: {
			if withinTolerance(nav.Heading(), com.targetHeading, angleTolerance) {
				com.state = maneuverLinear
				log.Printf("Heading within tolerance: theta=%f, theta_0=%f\n", nav.Heading(), com.targetHeading)
				log.Println("Started maneuver: linear")
			} else {
				return 0, maxOmega
			}
		}
		fallthrough
		case maneuverLinear: {
			predPos := predictPosition(nav.Position(), nav.Velocity())
			dist := distance(predPos, com.targetPosition)
			if withinTolerance(dist, 0, distanceTolerance) {
				com.state = maneuverDone
				log.Println("Movement done")
			} else {
				return maxVelocity, 0
			}
		}
		fallthrough
		case maneuverDone:
			return 0, 0
		default: // Что-то пошло ужасно неправильно
			return 0, 0
	}
}

func withinTolerance(val float64, target float64, tol float64) bool {
	return math.Abs(target - val) < tol
}

func distance(x vectors.Vector2, y vectors.Vector2) float64 {
	y.Sub(x)
	return y.Magnitude()
}

func radToDeg(rad float64) float64 {
	return rad * 180/math.Pi
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
