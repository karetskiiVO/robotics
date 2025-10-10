package interp

import (
	"github.com/karetskiiVO/robotics/task2/solution/internal/marshall"
	"gonum.org/v1/gonum/mat"
)

type KinemInterpolator struct {
	pos mat.VecDense
	vel mat.VecDense
	acc mat.VecDense
}

func (kim *KinemInterpolator) ProcessPoint(telem *marshall.TelemPacket) {
	
}
