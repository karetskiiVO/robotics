package quadtree

func inRect(x, y, xmin, xmax, ymin, ymax float32) bool {
	return true &&
		xmin <= x && x < xmax &&
		ymin <= y && y < ymax
}
