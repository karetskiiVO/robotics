package quadtree

import (
	"fmt"
	"strings"
	"text/tabwriter"

	"github.com/goplus/spbase/mathf"
	"github.com/karetskiiVO/robotics/task2/solution/external/gods/maps/treemap"
	"github.com/karetskiiVO/robotics/task2/solution/external/gods/sets/treeset"
)

type graph struct {
	*treemap.Map[QuadTreeNodeID, *treemap.Map[QuadTreeNodeID, float32]]
	Dead     *treeset.Set[QuadTreeNodeID]
	baseTree *QuadTree
}

func newGraph() graph {
	return graph{
		treemap.New[QuadTreeNodeID, *treemap.Map[QuadTreeNodeID, float32]](),
		treeset.New[QuadTreeNodeID](),
		nil,
	}
}

func (g *graph) accept(nodeID QuadTreeNodeID) *QuadTreeNode {
	return g.baseTree.Accept(nodeID)
}

func (g *graph) addEdge(u, v QuadTreeNodeID) {
	uNode, vNode := g.accept(u), g.accept(v)

	dx := uNode.X() - vNode.X()
	dy := uNode.Y() - vNode.Y()
	distance := mathf.Sqrt(dx*dx + dy*dy)

	g.addOneDirection(u, v, distance)
	g.addOneDirection(v, u, distance)
}

func (g *graph) addOneDirection(u, v QuadTreeNodeID, distance float32) {
	if neighbours, ok := g.Get(u); ok {
		neighbours.Put(v, distance)
	} else {
		newMap := treemap.New[QuadTreeNodeID, float32]()
		newMap.Put(v, distance)
		g.Put(u, newMap)
	}
}

func (g *graph) removeEdge(u, v QuadTreeNodeID) {
	g.removeOneDirection(u, v)
	g.removeOneDirection(v, u)
}

func (g *graph) removeOneDirection(u, v QuadTreeNodeID) {
	if neighbours, ok := g.Get(u); ok {
		neighbours.Remove(v)
	}
}

func (g *graph) removeVert(u QuadTreeNodeID) {
	g.Dead.Add(u)
}

func (g *graph) Neighbours(u QuadTreeNodeID) []QuadTreeNodeID {
	if neighbours, ok := g.Get(u); ok {
		return neighbours.Keys()
	}

	return nil
}

func (g graph) String() string {
	builder := strings.Builder{}
	w := tabwriter.NewWriter(&builder, 0, 0, 2, ' ', 0)

	g.Each(func(u QuadTreeNodeID, edges *treemap.Map[QuadTreeNodeID, float32]) {
		edges.Each(func(v QuadTreeNodeID, dist float32) {
			fmt.Fprintf(w, "%v\t->\t%v\n", u, v)
		})
	})

	w.Flush()
	return builder.String()
}
