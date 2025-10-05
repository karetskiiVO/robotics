package quadtree

func inRect(x, y, xmin, xmax, ymin, ymax float32) bool {
	return true &&
		xmin <= x && x < xmax &&
		ymin <= y && y < ymax
}

func touches(qn1, qn2 *QuadTreeNode) bool {
	verticalTouch := (qn1.xmax == qn2.xmin || qn2.xmax == qn1.xmin) &&
		(qn1.ymin < qn2.ymax && qn1.ymax > qn2.ymin)

	horizontalTouch := (qn1.ymax == qn2.ymin || qn2.ymax == qn1.ymin) &&
		(qn1.xmin < qn2.xmax && qn1.xmax > qn2.xmin)

	return verticalTouch || horizontalTouch
}
