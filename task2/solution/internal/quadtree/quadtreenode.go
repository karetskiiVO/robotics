package quadtree

// Zero value must be invalid
type QuadTreeNodeID int

type QuadTreeNode struct {
	// center point
	xmin   float32
	xmax   float32
	ymin   float32
	ymax   float32
	childs [2][2]QuadTreeNodeID
}

func (n QuadTreeNode) IsTerminal() bool {
	return n.childs[0][0] == 0
}

func (n QuadTreeNode) X() float32 {
	return (n.xmin + n.xmax) / 2
}

func (n QuadTreeNode) Y() float32 {
	return (n.ymin + n.ymax) / 2
}

func (n QuadTreeNode) RX() float32 {
	return (n.xmax - n.xmin) / 2
}

func (n QuadTreeNode) RY() float32 {
	return (n.ymax - n.ymin) / 2
}

func (n QuadTreeNode) XMin() float32 {
	return n.xmin
}

func (n QuadTreeNode) XMax() float32 {
	return n.xmax
}

func (n QuadTreeNode) YMin() float32 {
	return n.ymin
}

func (n QuadTreeNode) YMax() float32 {
	return n.ymax
}
