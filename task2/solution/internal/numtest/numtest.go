package numtest

import (
//	"fmt"
	"image/color"
	"math/rand"
	"math"
	"github.com/karetskiiVO/robotics/task2/solution/internal/navfront"
	"gonum.org/v1/plot"
	"gonum.org/v1/plot/plotter"
)

type NumTester interface {
	Function(float64) float64
}

type LinTester struct {
	Slope float64
	Intercept float64
	RandCoeff float64
	rnd *rand.Rand
}

type QuadTester struct {
	a float64
	b float64
	c float64
	RandCoeff float64
	rnd *rand.Rand
}

func NewLinTester(slope float64, inter float64, coeff float64) *LinTester {
	t := new(LinTester)
	src := rand.NewSource(42)
	t.rnd = rand.New(src)
	t.rnd.Float64()
	t.Slope = slope
	t.Intercept = inter
	t.RandCoeff = coeff
	return t
}

func (t *LinTester) Function(x float64) float64 {
	return x*t.Slope + t.Intercept + t.RandCoeff*(t.rnd.Float64() - 0.5)
}

func NewQuadTester(a float64, b float64, c float64, coeff float64) *QuadTester {
	t := new(QuadTester)
	t.a, t.b, t.c = a, b, c
	src := rand.NewSource(69)
	t.rnd = rand.New(src)
	t.RandCoeff = coeff
	return t
}

func (t *QuadTester) Function(x float64) float64 {
	return t.a*math.Pow(x, 2) + t.b*x + t.c + t.RandCoeff*(t.rnd.Float64() - 0.5)
}

func generate(tester NumTester, xs []float64) []float64 {
	ys := make([]float64, len(xs))
	for i, x := range xs {
		ys[i] = tester.Function(x)
	}
	return ys
}

func TestGH(tester NumTester, xs []float64) ([]float64, []float64, []float64) {
	filter := navfront.NewGHFilter(0.4, 0.1, 0.005)
	measurements := generate(tester, xs)
	step := xs[1] - xs[0]

	estims, preds := make([]float64, len(xs)), make([]float64, len(xs))
	for i, meas := range measurements[1:] {
		pred := filter.GhPredict(step)	
		filter.GhEstimate(meas, step)
		estims[i] = filter.Estimation().X
		preds[i] = pred.X
	}
	return measurements, estims, preds
}

func makePoints(xs []float64, ys []float64) plotter.XYs {
	pts := make(plotter.XYs, len(xs))
	for i := range xs {
		pts[i].X = xs[i]
		pts[i].Y = ys[i]
	}
	return pts
}

func makeLine(xys plotter.XYs) (*plotter.Line, *plotter.Scatter) {
	line, scatter, err := plotter.NewLinePoints(xys)
	if err != nil {
		panic(err)
	}
	return line, scatter
}

func plotAddLine(plt *plot.Plot, xs []float64, ys[]float64, col color.Color) {
	pts := makePoints(xs, ys)
	line, scatter := makeLine(pts)
	line.Color = col
	plt.Add(line, scatter)
}

func PlotGH(tester NumTester, xs []float64) *plot.Plot {
	red := color.RGBA{255, 0, 0, 255}
	green := color.RGBA{0, 255, 0, 255}
	blue := color.RGBA{0, 0, 255, 255}

	meas, estim, pred := TestGH(tester, xs)
	plt := plot.New()
	
	plt.Add(plotter.NewGrid())
	plotAddLine(plt, xs, meas, red)
	plotAddLine(plt, xs, estim, green)
	plotAddLine(plt, xs, pred, blue)
	return plt
}
