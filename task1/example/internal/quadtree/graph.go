package quadtree

import (
	"github.com/goplus/spbase/mathf"
	"github.com/karetskiiVO/robotics/task1/example/external/gods/maps/treemap"
)

type graph struct {
	*treemap.Map[QuadTreeNodeID, *treemap.Map[QuadTreeNodeID, float32]]
	baseTree *QuadTree
}

func NewGraph() graph {
	return graph{
		treemap.New[QuadTreeNodeID, *treemap.Map[QuadTreeNodeID, float32]](),
		nil,
	}
}

func (g *graph) accept(nodeID QuadTreeNodeID) *QuadTreeNode {
	return g.baseTree.accept(nodeID)
}

func (g *graph) AddEdge(u, v QuadTreeNodeID) {
	uNode, vNode := g.accept(u), g.accept(v)

	dx := uNode.x() - vNode.x()
	dy := uNode.y() - vNode.y()
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

func (g *graph) RemoveEdge(u, v QuadTreeNodeID) {
	g.removeOneDirection(u, v)
	g.removeOneDirection(v, u)
}

func (g *graph) removeOneDirection(u, v QuadTreeNodeID) {
	if neighbours, ok := g.Get(u); ok {
		neighbours.Remove(v)
	}
}

func (g *graph) RemoveVert(u QuadTreeNodeID) {
	if neighbours, ok := g.Get(u); ok {
		neighbours.Each(func(v QuadTreeNodeID, _ float32) {
			g.removeOneDirection(v, u)
		})

		g.Remove(u)
	}
}

func (g *graph) Neighbours(u QuadTreeNodeID) []QuadTreeNodeID {
	if neighbours, ok := g.Get(u); ok {
		return neighbours.Keys()
	}

	return nil
}
