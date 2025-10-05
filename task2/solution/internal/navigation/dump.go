package navigation

import (
	"context"
	"image"
	"image/color"

	"github.com/hajimehoshi/ebiten/v2"
	"github.com/karetskiiVO/robotics/task2/solution/internal/quadtree"
	"github.com/phpgao/tlog"
)

func (s *System) dump(ctx context.Context) {
	dumper := newDumper(s)

	ebiten.SetWindowTitle("navigation dump")
	if err := ebiten.RunGame(dumper); err != nil {
		tlog.Error(err)
	}
}

// TODO: сделать приближалку

type dumper struct {
	s *System
}

func newDumper(s *System) *dumper {
	return &dumper{s}
}

func (d *dumper) Update() error {
	return nil
}

func (d *dumper) Draw(screen *ebiten.Image) {
	screen.Fill(color.RGBA{R: 30, G: 30, B: 50, A: 255})

	bounds := screen.Bounds()
	treeScreen := screen.SubImage(image.Rect(
		bounds.Min.X,
		bounds.Min.Y,
		bounds.Min.X+bounds.Dx()/2,
		bounds.Min.Y+bounds.Dy()/2,
	)).(*ebiten.Image)

	d.s.mu.Lock()
	d.drawTree(treeScreen, d.s.tree)
	d.s.mu.Unlock()
}

func (d *dumper) Layout(outsideWidth, outsideHeight int) (screenWidth, screenHeight int) {
	return 800, 600
}

func drawLine(img *ebiten.Image, x1, y1, x2, y2 int, clr color.Color) {
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

func abs(x int) int {
	if x < 0 {
		return -x
	}
	return x
}

func (d *dumper) drawTree(screen *ebiten.Image, tree *quadtree.QuadTree) {
	width, height := screen.Size()

	xmin := d.s.tree.XMin()
	xmax := d.s.tree.XMax()
	ymin := d.s.tree.YMin()
	ymax := d.s.tree.YMax()

	sizex := float32(min(width, height)) / (xmax - xmin)
	sizey := float32(min(width, height)) / (ymax - ymin)

	for _, node := range tree.Nodes() {
		drawLine(screen,
			int((node.XMin()-xmin)*sizex),
			int((node.YMin()-ymin)*sizey),
			int((node.XMin()-xmin)*sizex),
			int((node.YMax()-ymin)*sizey),
			color.Gray{128},
		)
		drawLine(screen,
			int((node.XMin()-xmin)*sizex),
			int((node.YMin()-ymin)*sizey),
			int((node.XMax()-xmin)*sizey),
			int((node.YMin()-ymin)*sizex),
			color.Gray{128},
		)
		drawLine(screen,
			int((node.XMax()-xmin)*sizex),
			int((node.YMax()-ymin)*sizey),
			int((node.XMin()-xmin)*sizey),
			int((node.YMax()-ymin)*sizex),
			color.Gray{128},
		)
		drawLine(screen,
			int((node.XMax()-xmin)*sizex),
			int((node.YMax()-ymin)*sizey),
			int((node.XMax()-xmin)*sizey),
			int((node.YMin()-ymin)*sizex),
			color.Gray{128},
		)
	}

	// tree.Each(func(u quadtree.QuadTreeNodeID, neighbour *treemap.Map[quadtree.QuadTreeNodeID, float32]) {
	// 	neighbour.Each(func(v quadtree.QuadTreeNodeID, _ float32) {
	// 		uNode := tree.Accept(u)
	// 		vNode := tree.Accept(v)

	// 		drawLine(screen,
	// 			int((uNode.X()-xmin)*sizex),
	// 			int((uNode.Y()-ymin)*sizey),
	// 			int((vNode.X()-xmin)*sizex),
	// 			int((vNode.Y()-ymin)*sizey),
	// 			color.RGBA{0, 255, 255, 255},
	// 		)
	// 	})
	// })
}
