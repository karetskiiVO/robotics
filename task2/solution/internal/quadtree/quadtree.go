package quadtree

import (
	"github.com/karetskiiVO/robotics/task1/example/external/gods/maps/treemap"
)

const (
	rootID    QuadTreeNodeID = 0
	InvalidID QuadTreeNodeID = -1
)

type QuadTree struct {
	precision float32

	// ROOT всегда на нулевой позиции
	nodes []QuadTreeNode

	graph
}

func New(x1, y1, x2, y2, precision float32) *QuadTree {
	root := QuadTreeNode{
		xmin: min(x1, x2),
		xmax: max(x1, x2),
		ymin: min(y1, y2),
		ymax: max(y1, y2),
	}

	leafGraph := newGraph()
	leafGraph.Put(rootID, treemap.New[QuadTreeNodeID, float32]())

	tree := &QuadTree{
		precision: precision,
		graph:     leafGraph,
	}

	tree.baseTree = tree
	tree.newQuadTreeNode(root)

	return tree
}

func (q *QuadTree) Accept(id QuadTreeNodeID) *QuadTreeNode {
	return &q.nodes[id]
}

// не добавляет в граф, просто аллоцирует вершину
func (q *QuadTree) newQuadTreeNode(node QuadTreeNode) QuadTreeNodeID {
	q.nodes = append(q.nodes, node)
	return QuadTreeNodeID(len(q.nodes) - 1)
}

// func (q *QuadTree) free(id QuadTreeNodeID) {
// 	q.freeID = append(q.freeID, id)
// }

func (q *QuadTree) Find(x, y float32) (QuadTreeNodeID, bool) {
	root := q.Accept(rootID)
	if !inRect(x, y, root.xmin, root.xmax, root.ymin, root.ymax) {
		return InvalidID, false
	}

	return q.find(rootID, x, y, false), true
}

// Для разбиения выставляем квад true
func (q *QuadTree) find(nodeID QuadTreeNodeID, x, y float32, withQuad bool) QuadTreeNodeID {
	for {
		node := q.Accept(nodeID)
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
				node = q.Accept(nodeID)
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
	root := q.Accept(rootID)
	if !inRect(x, y, root.xmin, root.xmax, root.ymin, root.ymax) {
		return
	}

	nodeID := q.find(rootID, x, y, true)
	q.removeVert(nodeID)
}

func (q *QuadTree) quadNode(nodeID QuadTreeNodeID) {
	node := q.Accept(nodeID)

	nodex, nodey := node.X(), node.Y()

	/// MAYBE BUG
	createdChilds := [2][2]QuadTreeNodeID{
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
	node = q.Accept(nodeID)
	node.childs = createdChilds

	neighboursIDs := q.Neighbours(nodeID)
	q.removeVert(nodeID)

	q.addEdge(node.childs[0][0], node.childs[0][1])
	q.addEdge(node.childs[0][1], node.childs[1][1])
	q.addEdge(node.childs[1][1], node.childs[1][0])
	q.addEdge(node.childs[1][0], node.childs[0][0])

	childIDs := [...]QuadTreeNodeID{
		node.childs[0][0],
		node.childs[0][1],
		node.childs[1][0],
		node.childs[1][1],
	}
	childs := [...]*QuadTreeNode{
		q.Accept(childIDs[0]),
		q.Accept(childIDs[1]),
		q.Accept(childIDs[2]),
		q.Accept(childIDs[3]),
	}

	// надо распихать теперь соседей
	for _, neighbourID := range neighboursIDs {
		neighbour := q.Accept(neighbourID)
		// По идее проблем быть не должно, а так хз

		for idx, child := range childs {
			if touches(neighbour, child) {
				q.addEdge(neighbourID, childIDs[idx])
			}
		}
	}
}

func (q QuadTree) Nodes() []QuadTreeNode {
	return q.nodes
}

func (q QuadTree) XMin() float32 {
	return q.nodes[rootID].xmin
}

func (q QuadTree) XMax() float32 {
	return q.nodes[rootID].xmax
}

func (q QuadTree) YMin() float32 {
	return q.nodes[rootID].ymin
}

func (q QuadTree) YMax() float32 {
	return q.nodes[rootID].ymax
}
