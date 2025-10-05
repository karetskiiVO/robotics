package transform

type Vec3 struct {
	X, Y, Z float32
}

func NewVec3(coords ...float32) Vec3 {
	panic("")
}

type Vec2 struct {
	X, Y float32
}

type RawPath []Vec2

func NewVec2(coords ...float32) Vec2 {
	switch len(coords) {
	case 0:
		return Vec2{}
	case 1:
		return Vec2{
			X: coords[0],
		}
	default:
		return Vec2{
			X: coords[0],
			Y: coords[1],
		}
	}
}

func (v Vec2) Unpak() (float32, float32) {
	return v.X, v.Y
}

type Transform struct {
	Position Vec2
	Thetha   float32
}
