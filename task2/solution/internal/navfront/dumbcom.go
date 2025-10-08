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
	maneuverTurn Maneuver = 1
	maneuverFullstop Maneuver = 2
	maneuverLine Maneuver = 3
)

type DumbCommander struct {
	state Maneuver
	prevState Maneuver
	targetPosition vectors.Vector2
	targetHeading float64
	channel chan State
}

type State struct {
	V float64
	Omega float64
}

const angleTolerance float64 = 5e-2
const distanceTolerance float64 = 1e-1
const omegaTolerance float64 = 1e-3

const maxVelocity float64 = 0.5 * 2
const maxOmega float64 = 1.0 * 2
const maxLinearAcc float64 = 0.05 * 2
const maxAngularAcc float64 = 0.2 * 2

func (com *DumbCommander) MoveTo(point vectors.Vector2) {
	nav := navigation.NavInstance()
	currentPos := nav.Position()
	com.targetPosition = point
	log.Printf("TargPos: %v\n", com.targetPosition)

	point.Sub(currentPos)
	com.targetHeading = point.AngleRadians()
	log.Printf("TargHeading: %f deg, deltaR: %v {m, m}\n", radToDeg(com.targetHeading), point)

	com.setState(maneuverTurn)
}

func (com *DumbCommander) Loop() {
	com.channel = make(chan State)
	for {
		switch com.state {
			case maneuverTurn:
				engageTurn(com.targetHeading, com.channel)
				com.setState(maneuverFullstop)
			case maneuverFullstop:
				engageFullstop(com.channel)
				if com.prevState == maneuverTurn {
					com.setState(maneuverLine)
				} else {
					com.setState(maneuverDone)
					log.Println("Movement done")
				}
			case maneuverLine:
				engageLine(com.targetPosition, com.channel)
				com.setState(maneuverFullstop)
			case maneuverDone:
				continue
		}
	}
}

func (com *DumbCommander) setState(state Maneuver) {
	com.prevState = com.state
	com.state = state
}

func (com *DumbCommander) Step() (float64, float64) {
	if com.channel == nil {
		return 0, 0
	}
	state := <- com.channel	
	v, omega := state.V, state.Omega
	return v, omega
}

func engageTurn(targetHeading float64, channel chan State) {
	nav := navigation.NavInstance()
	var omega float64
	if nav.Heading() < targetHeading {
		omega = maxOmega
	} else {
		omega = -maxOmega
	}
	channel <- State{0, omega}
	log.Printf("Started maneuver Turn to theta=%f deg\n", radToDeg(targetHeading))
	for {
		predHeading := predictHeading(nav.Heading(), nav.AngularVelocity().Y)
		log.Printf("%f\n", radToDeg(predHeading))
		if withinTolerance(predHeading, targetHeading, angleTolerance) {
			break
		}
	}
	log.Printf("Finished maneuver Turn at %f\n", radToDeg(nav.Heading()))
}

func engageFullstop(channel chan State) {
	nav := navigation.NavInstance()
	channel <- State{0, 0}
	log.Printf("Started maneuver Fullstop\n")
	for {	
		if withinTolerance(nav.AngularVelocity().Y, 0, omegaTolerance) {
			break
		}
	}
	log.Printf("Finished maneuver Fullstop at %f\n", radToDeg(nav.Heading()))
}

func engageLine(targetPosition vectors.Vector2, channel chan State) {
	nav := navigation.NavInstance()
	channel <- State{maxVelocity, 0}
	log.Printf("Started maneuver Line\n")
	for {
		predPosition := predictPosition(nav.Position(), nav.Velocity())
		dist := distance(predPosition, targetPosition)
		if withinTolerance(dist, 0, distanceTolerance) {
			log.Printf("Finished maneuver Line at d=%f\n", dist)
			break	
		}
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

func predictPosition(pos vectors.Vector2, vel vectors.Vector2) vectors.Vector2 {
	displMagn := 3*vel.MagnitudeSquared() / (2*maxLinearAcc)
	angle := vel.AngleRadians()
	r := vectors.Vector2{
		X: pos.X + displMagn*math.Sin(angle),
		Y: pos.Y + displMagn*math.Cos(angle),
	}
	return r
}

func predictHeading(heading float64, omega float64) float64 {
	return heading + 3*math.Pow(omega, 2) / (2*maxAngularAcc)
}
