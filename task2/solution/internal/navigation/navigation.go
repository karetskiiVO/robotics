package navigation

import (
	"context"
	"fmt"
	"math"
	"slices"
	"sync"

	"github.com/goplus/spbase/mathf"
	"github.com/karetskiiVO/robotics/task2/solution/external/gods/maps/treemap"
	"github.com/karetskiiVO/robotics/task2/solution/internal/quadtree"
	"github.com/karetskiiVO/robotics/task2/solution/internal/robobpacket"
	"github.com/karetskiiVO/robotics/task2/solution/internal/transform"
)

var withDebug = false

const eps = 1e-6

type System struct {
	tree      *quadtree.QuadTree
	Transform transform.Transform

	mu sync.Mutex
}

func New(ctx context.Context, x1, y1, x2, y2, precision float32) *System {
	s := &System{
		tree: quadtree.New(x1, y1, x2, y2, precision),
	}

	if withDebug {
		go s.dump(ctx)
	}

	return s
}

// или делать дипкопи для потокобезопасности
func (s *System) Update(telemetry *robobpacket.Telemetry) {
	s.mu.Lock()
	// TODO: более акууратно пересчитывать
	s.Transform.Position = transform.NewVec2(telemetry.Header.OdomX, telemetry.Header.OdomY)
	s.Transform.Thetha = telemetry.Header.OdomTh

	for i, disti := range telemetry.Lidar {
		// угол текущего луча
		phi := telemetry.Header.OdomTh +
			(mathf.Pi/4)*(float32(i)/float32(len(telemetry.Lidar)-1)-1)

		x := telemetry.Header.OdomX + disti*mathf.Cos(phi)
		y := telemetry.Header.OdomY + disti*mathf.Sin(phi)

		s.tree.Prick(x, y)
	}
	s.mu.Unlock()
}

func (s *System) TraceToTarget(target transform.Vec2) (transform.RawPath, error) {
	s.mu.Lock()
	defer s.mu.Unlock()

	// TODO: проверка на истинность
	curID, _ := s.tree.Find(s.Transform.Position.Unpak())
	targetID, _ := s.tree.Find(target.Unpak())

	dists := treemap.New[float32, quadtree.QuadTreeNodeID]()
	dists.Put(0, curID)

	used := treemap.New[quadtree.QuadTreeNodeID, float32]()

	for dists.Size() > 0 {
		distu, u, _ := dists.Min()
		dists.Remove(distu)

		if _, ok := used.Get(u); ok {
			continue
		}
		used.Put(u, distu)

		if u == targetID {
			break
		}

		edges, _ := s.tree.Get(u)
		edges.Each(func(v quadtree.QuadTreeNodeID, distuv float32) {
			if _, ok := used.Get(v); ok {
				return
			}

			dists.Put(distu+distuv, v)
		})
	}

	targetDist, ok := used.Get(targetID)
	if !ok {
		return nil, fmt.Errorf("can't find a path (.x=%.2f .y=%.2f) -> (.x=%.2f .y=%.2f)",
			s.Transform.Position.X, s.Transform.Position.Y,
			target.X, target.Y,
		)
	}

	res := transform.RawPath{}

	v := targetID
	distv := targetDist
	res = append(res, target)
	for v != curID {
		next := quadtree.InvalidID
		distnext := float32(math.Inf(1))

		edges, _ := s.tree.Get(v)
		edges.Each(func(u quadtree.QuadTreeNodeID, distuv float32) {
			distu, ok := used.Get(u)
			if !ok {
				return
			}

			if distu+distuv-distv < eps {
				next = u
				distnext = distu
			}
		})

		vnode := s.tree.Accept(v)
		res = append(res, transform.NewVec2(vnode.X(), vnode.Y()))

		v = next
		distv = distnext
	}
	res = append(res, s.Transform.Position)
	slices.Reverse(res)

	return res, nil
}
