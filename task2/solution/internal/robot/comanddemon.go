package robot

import (
	"encoding/binary"

	"github.com/goplus/spbase/mathf"
	"github.com/karetskiiVO/robotics/task2/solution/internal/robobpacket"
	"github.com/karetskiiVO/robotics/task2/solution/internal/transform"
	"github.com/phpgao/tlog"
)

func (r *Robot) commandDemon() {
	buffer := make([]byte, 1024)

	target := transform.Transform{}

	for {
		r.nav.Mu.RLock()
		transform := r.nav.Transform
		r.nav.Mu.RUnlock()

		select {
		case <-r.ctx.Done():
			return
		case newtarget := <-r.target:
			target = newtarget
			tlog.DebugContextf(r.ctx, "new path (.x=%.2f .y=%.2f) -> (.x=%.2f .y=%.2f .th=%.2f)",
				transform.Position.X,
				transform.Position.Y,
				target.Position.X,
				target.Position.Y,
				target.Thetha,
			)
		default:
		}

		_ = transform
		_ = target

		// Сначала поворачиваеся

		command := robobpacket.Command{}
		diff := angleAbsDiff(transform.Thetha, target.Thetha)
		if diff < 1e-3 {
			command.Linear = 0.2
			command.Angular = mathf.Signf(diff)
		} else if target.Position.Sub(transform.Position).Mag2() < 1e-3 {
			command.Linear = 1
			command.Angular = 0
		}

		size, err := binary.Encode(buffer, binary.LittleEndian, command)
		if err != nil {
			tlog.ErrorContextf(r.ctx, "Error while encoding command: %v", err)
			continue
		}

		_, err = r.commandConn.Write(buffer[:size])
		if err != nil {
			tlog.ErrorContextf(r.ctx, "Error while sending command: %v", err)
			continue
		}
	}
}

func angleAbsDiff(a, b float32) float32 {
	diff := mathf.Absf(a - b)
	return min(diff, mathf.Absf(mathf.Tau-diff))
}
