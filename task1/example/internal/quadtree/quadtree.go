package quadtree

import (
	"math"

	"github.com/karetskiiVO/robotics/task1/example/external/gods/maps/treemap"
)

const (
	rootID    QuadTreeNodeID = 0
	invalidID QuadTreeNodeID = -1
)

type QuadTree struct {
	precision float32

	// ROOT всегда на нулевой позиции
	nodes  []QuadTreeNode
	freeID []QuadTreeNodeID

	leafGraph graph
}

func New(x1, y1, x2, y2, precision float32) *QuadTree {
	root := QuadTreeNode{
		xmin: min(x1, x2),
		xmax: max(x1, x2),
		ymin: min(y1, y2),
		ymax: max(y1, y2),
	}

	leafGraph := NewGraph()
	leafGraph.Put(rootID, treemap.New[QuadTreeNodeID, float32]())

	tree := &QuadTree{
		precision: precision,
		leafGraph: leafGraph,
	}

	tree.leafGraph.baseTree = tree
	tree.newQuadTreeNode(root)

	return tree
}

func (q *QuadTree) accept(id QuadTreeNodeID) *QuadTreeNode {
	return &q.nodes[id]
}

// не добавляет в граф, просто аллоцирует вершину
func (q *QuadTree) newQuadTreeNode(node QuadTreeNode) QuadTreeNodeID {
	if len(q.freeID) > 0 {
		id := q.freeID[len(q.freeID)-1]

		q.freeID = q.freeID[:len(q.freeID)-1]
		q.nodes[id] = node
		return id
	}

	q.nodes = append(q.nodes, node)
	return QuadTreeNodeID(len(q.nodes) - 1)
}

// func (q *QuadTree) free(id QuadTreeNodeID) {
// 	q.freeID = append(q.freeID, id)
// }

func (q *QuadTree) Find(x, y float32) (QuadTreeNodeID, bool) {
	root := q.accept(rootID)
	if !inRect(x, y, root.xmin, root.xmax, root.ymin, root.ymax) {
		return invalidID, false
	}

	return q.find(rootID, x, y, false), true
}

// Для разбиения выставляем квад true
func (q *QuadTree) find(nodeID QuadTreeNodeID, x, y float32, withQuad bool) QuadTreeNodeID {
	for {
		node := q.accept(nodeID)
		xmin := node.xmin
		xmax := node.xmax
		ymin := node.ymin
		ymax := node.ymax

		if max(xmax-xmin, ymax-ymin) < q.precision {
			return nodeID
		}

		if node.IsTerminal() {
			if withQuad {
				q.quadNode(nodeID)
			} else {
				return nodeID
			}
		}

		xmid := (xmin + xmax) / 2
		ymid := (ymin + ymax) / 2

		xIdx, yIdx := -1, -1

		if x < xmid {
			xmax = xmid
			xIdx = 0
		} else {
			xmin = xmid
			xIdx = 1
		}

		if y < ymid {
			ymax = ymid
			yIdx = 0
		} else {
			ymin = ymid
			yIdx = 1
		}

		nodeID = node.childs[xIdx][yIdx]
	}
}

func (q *QuadTree) Prick(x, y float32) {
	root := q.accept(rootID)
	if !inRect(x, y, root.xmin, root.xmax, root.ymin, root.ymax) {
		return
	}

	nodeID := q.find(rootID, x, y, true)
	q.leafGraph.RemoveVert(nodeID)
}

func (q *QuadTree) quadNode(nodeID QuadTreeNodeID) {
	node := q.accept(nodeID)

	nodex, nodey := node.x(), node.y()

	/// MAYBE BUG
	node.childs = [2][2]QuadTreeNodeID{
		{
			q.newQuadTreeNode(QuadTreeNode{
				xmin: node.xmin,
				ymin: node.ymin,
				xmax: nodex,
				ymax: nodey,
			}),
			q.newQuadTreeNode(QuadTreeNode{
				xmin: node.xmin,
				ymin: nodey,
				xmax: nodex,
				ymax: node.ymax,
			}),
		},
		{
			q.newQuadTreeNode(QuadTreeNode{
				xmin: nodex,
				ymin: node.ymin,
				xmax: node.xmax,
				ymax: nodey,
			}),
			q.newQuadTreeNode(QuadTreeNode{
				xmin: nodex,
				ymin: nodey,
				xmax: node.xmax,
				ymax: node.ymax,
			}),
		},
	}

	neighboursIDs := q.leafGraph.Neighbours(nodeID)
	q.leafGraph.RemoveVert(nodeID)

	q.leafGraph.AddEdge(node.childs[0][0], node.childs[0][1])
	q.leafGraph.AddEdge(node.childs[0][1], node.childs[1][1])
	q.leafGraph.AddEdge(node.childs[1][1], node.childs[1][0])
	q.leafGraph.AddEdge(node.childs[1][0], node.childs[0][0])

	childIDs := [...]QuadTreeNodeID{
		node.childs[0][0],
		node.childs[0][1],
		node.childs[1][0],
		node.childs[1][1],
	}
	childs := [...]*QuadTreeNode{
		q.accept(childIDs[0]),
		q.accept(childIDs[1]),
		q.accept(childIDs[2]),
		q.accept(childIDs[3]),
	}

	// надо распихать теперь соседей
	for _, neighbourID := range neighboursIDs {
		neighbour := q.accept(neighbourID)
		// По идее проблем быть не должно, а так хз
		neighbourx := neighbour.x()
		neighboury := neighbour.y()

		mindist := float32(math.Inf(1))

		for _, child := range childs {
			dx := (neighbourx - child.x())
			dy := (neighboury - child.y())
			dist := dx*dx + dy*dy

			mindist = min(mindist, dist)
		}

		for childIdx, child := range childs {
			dx := (neighbourx - child.x())
			dy := (neighboury - child.y())
			dist := dx*dx + dy*dy

			if dist-mindist < 0.1*q.precision {
				q.leafGraph.AddEdge(neighbourID, childIDs[childIdx])
			}
		}
	}
}
