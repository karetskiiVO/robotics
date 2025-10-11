package navfront

import (
	"math"
	"time"
	"gonum.org/v1/gonum/spatial/r3"
	"gonum.org/v1/gonum/spatial/r2"
	"github.com/karetskiiVO/robotics/task2/solution/internal/marshall"
)

type GHFilter struct {
	coeffs r3.Vec
	estimation r3.Vec
}

func NewGHFilter(alpha float64, beta float64, gamma float64) *GHFilter {
	ghf := new(GHFilter)
	ghf.coeffs.X = alpha
	ghf.coeffs.Y = beta
	ghf.coeffs.Z = gamma
	return ghf
}

type  GHNavigator struct {
	ghfX *GHFilter
	ghfY *GHFilter
	ghfTheta *GHFilter
	prevStepUs float64
}

func NewGHNavigator() *GHNavigator {
	nav := new(GHNavigator)
	nav.ghfX = NewGHFilter(0.4, 0.1, 0.005)
	nav.ghfY = NewGHFilter(0.4, 0.1, 0.005)
	nav.ghfTheta = NewGHFilter(0.4, 0.1, 0.005)
	return nav
}

func (nav *GHNavigator) Step(telem *marshall.TelemPacket) {
	timeNow := float64(time.Now().UnixMicro())
	deltaT := timeNow - nav.prevStepUs
	nav.ghfX.GhEstimate(float64(telem.Header.OdomX), deltaT)
	nav.ghfY.GhEstimate(float64(telem.Header.OdomY), deltaT)
	nav.ghfTheta.GhEstimate(float64(telem.Header.OdomTh), deltaT)
	nav.prevStepUs = float64(timeNow)
}

func (nav *GHNavigator) Position() r2.Vec {
	return r2.Vec{
		X: nav.ghfX.estimation.X,
		Y: nav.ghfY.estimation.X,
	}
}

func (nav *GHNavigator) Velocity() r2.Vec {
	return r2.Vec{
		X: nav.ghfX.estimation.X,
		Y: nav.ghfY.estimation.X,
	}
}

func (nav *GHNavigator) Heading() float64 {
	return nav.ghfTheta.estimation.X
}

func (nav *GHNavigator) AngularVelocity() float64 {
	return nav.ghfTheta.estimation.Y
}

/*
	x_{k+1}=x_k+\alpha*r_k
	v_{k+1}=v_k+\frac{\beta}{\DeltaT}r_k
	a_{k+1}=a_k+\frac{2\gamma}{\DeltaT^2}r_k
*/
func (ghf *GHFilter)GhEstimate(meas float64, deltaT float64) {
	// Предсказание
	pred := ghf.GhPredict(deltaT)
	// Обновление
	residual := meas - pred.X
	resVec := r3.Vec{
		X: residual,
		Y: residual,
		Z: residual, 
	}
	scaledCoeffs := vecMulElem(ghf.coeffs, r3.Vec{
		X: 1,
		Y: 1/deltaT,
		Z: 2/math.Pow(deltaT, 2),
	})
	ghf.estimation = r3.Add(pred, vecMulElem(scaledCoeffs, resVec))
}

func (ghf *GHFilter) GhPredict(deltaT float64) r3.Vec {
	x, v, a := ghf.estimation.X, ghf.estimation.Y, ghf.estimation.Z	
	return r3.Vec{
		X: x + v*deltaT + a*math.Pow(deltaT, 2)/2,
		Y: v + a*deltaT,
		Z: a,
	}
}

func (ghf *GHFilter) Estimation() r3.Vec {
	return ghf.estimation
}

func vecMulElem(v r3.Vec, u r3.Vec) r3.Vec {
	return r3.Vec{
		X: v.X*u.X,
		Y: v.Y*u.Y,
		Z: v.Z*u.Z,
	}
}
