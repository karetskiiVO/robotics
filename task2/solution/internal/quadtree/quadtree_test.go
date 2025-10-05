package quadtree

import (
	"math"
	"math/rand"
	"time"

	"github.com/hajimehoshi/ebiten/v2"
	"github.com/karetskiiVO/robotics/task1/example/external/gods/maps/treemap"

	"image/color"
	"testing"
)

var points []struct{ x, y float32 }

type Game struct {
	quadro     *QuadTree
	lastUpdate time.Time
}

func (g *Game) Update() error {
	if time.Since(g.lastUpdate) > time.Second/100 && len(points) < 20 {
		g.lastUpdate = time.Now()

		x, y := rand.Float32(), rand.Float32()

		g.quadro.Prick(x, y)
		points = append(points,
			struct {
				x float32
				y float32
			}{x, y},
		)
	}

	return nil
}

func (g *Game) Draw(screen *ebiten.Image) {
	drawQuadro(screen, g.quadro)
}

func (g *Game) Layout(outsideWidth, outsideHeight int) (screenWidth, screenHeight int) {
	return 800, 600
}

// Функция для рисования круга
func drawCircle(screen *ebiten.Image, x, y, radius int, clr color.Color) {
	for dy := -radius; dy <= radius; dy++ {
		for dx := -radius; dx <= radius; dx++ {
			if dx*dx+dy*dy <= radius*radius {
				screen.Set(x+dx, y+dy, clr)
			}
		}
	}
}

func TestQuadTree(t *testing.T) {
	game := &Game{
		quadro:     New(0, 0, 1, 1, 0.1),
		lastUpdate: time.Now(),
	}

	ebiten.SetWindowTitle("Ebiten Graphics Demo")
	if err := ebiten.RunGame(game); err != nil {
		panic(err)
	}
}

func drawQuadro(screen *ebiten.Image, tree *QuadTree) {
	width, height := screen.Size()

	size := float32(min(width, height))
	screen.Fill(color.RGBA{R: 30, G: 30, B: 50, A: 255})

	for _, node := range tree.nodes {
		DrawLineBresenham(screen,
			int(node.xmin*size),
			int(node.ymin*size),
			int(node.xmin*size),
			int(node.ymax*size),
			color.Gray{128},
		)
		DrawLineBresenham(screen,
			int(node.xmin*size),
			int(node.ymin*size),
			int(node.xmax*size),
			int(node.ymin*size),
			color.Gray{128},
		)
		DrawLineBresenham(screen,
			int(node.xmax*size),
			int(node.ymax*size),
			int(node.xmin*size),
			int(node.ymax*size),
			color.Gray{128},
		)
		DrawLineBresenham(screen,
			int(node.xmax*size),
			int(node.ymax*size),
			int(node.xmax*size),
			int(node.ymin*size),
			color.Gray{128},
		)
	}

	for _, point := range points {
		drawCircle(screen,
			int(point.x*size),
			int(point.y*size),
			5,
			color.RGBA{255, 0, 0, 255},
		)
	}

	tree.Each(func(u QuadTreeNodeID, neighbour *treemap.Map[QuadTreeNodeID, float32]) {
		neighbour.Each(func(v QuadTreeNodeID, _ float32) {
			uNode := tree.Accept(u)
			vNode := tree.Accept(v)

			DrawThickLine(screen,
				int(uNode.X()*size),
				int(uNode.Y()*size),
				int(vNode.X()*size),
				int(vNode.Y()*size),
				3,
				color.RGBA{0, 255, 255, 255},
			)
		})
	})
}

func DrawLineBresenham(img *ebiten.Image, x1, y1, x2, y2 int, clr color.Color) {
	dx := abs(x2 - x1)
	dy := abs(y2 - y1)

	var sx, sy int
	if x1 < x2 {
		sx = 1
	} else {
		sx = -1
	}
	if y1 < y2 {
		sy = 1
	} else {
		sy = -1
	}

	err := dx - dy

	for {
		img.Set(x1, y1, clr)

		if x1 == x2 && y1 == y2 {
			break
		}

		e2 := 2 * err
		if e2 > -dy {
			err -= dy
			x1 += sx
		}
		if e2 < dx {
			err += dx
			y1 += sy
		}
	}
}

func DrawThickLine(img *ebiten.Image, x1, y1, x2, y2, thickness int, clr color.Color) {
	DrawLineBresenham(img, x1, y1, x2, y2, clr)
	if thickness > 1 {
		dx := x2 - x1
		dy := y2 - y1

		length := math.Sqrt(float64(dx*dx + dy*dy))
		if length == 0 {
			return
		}

		nx := float64(-dy) / length
		ny := float64(dx) / length

		for t := 1; t < thickness; t++ {
			offsetX := int(nx * float64(t) / 2)
			offsetY := int(ny * float64(t) / 2)

			DrawLineBresenham(img, x1+offsetX, y1+offsetY, x2+offsetX, y2+offsetY, clr)
			DrawLineBresenham(img, x1-offsetX, y1-offsetY, x2-offsetX, y2-offsetY, clr)
		}
	}
}

func abs(x int) int {
	if x < 0 {
		return -x
	}
	return x
}
