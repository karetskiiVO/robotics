package navigation

import (
	"github.com/karetskiiVO/robotics/task1/example/internal/quadtree"
	"github.com/karetskiiVO/robotics/task1/example/internal/transform"
)

type System struct {
	tree      *quadtree.QuadTree
	Transform transform.Transform
}

func New(x1, y1, x2, y2 float32, precision float32) System {
	return System{
		tree: quadtree.New(x1, y1, x2, y2, precision),
	}
}
