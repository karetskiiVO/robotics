package navfront

import (
	"math"
	"gonum.org/v1/gonum/mat"
	"github.com/karetskiiVO/robotics/task2/solution/internal/marshall"
)

type GHFilter struct {
	coeffs *mat.VecDense
	estimation *mat.VecDense
}

type  GHNavigator struct {
}

func (nav *GHNavigator) Step(telem *marshall.TelemPacket) {

}

func (nav *GHNavigator) Predict(deltaT int64) {

}

/*
	x_{k+1}=x_k+\alpha*r_k
	v_{k+1}=v_k+\frac{\beta}{\DeltaT}r_k
	a_{k+1}=a_k+\frac{2\gamma}{\DeltaT^2}r_k
*/
func (ghf *GHFilter)ghEstimate(meas mat.Vector, pred mat.Vector, deltaT float64) {
	residual := mat.NewVecDense(2, nil)
	residual.SubVec(meas, pred)

	scaledCoeffs := mat.NewVecDense(2, []float64{1, 1/deltaT, math.Pow(1/deltaT, 2)})
	scaledCoeffs.MulElemVec(scaledCoeffs, ghf.coeffs) 
	residual.MulElemVec(scaledCoeffs, residual)

	ghf.estimation.AddVec(ghf.estimation, residual)
}

