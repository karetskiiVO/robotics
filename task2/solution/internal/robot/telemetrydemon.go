package robot

import (
	"github.com/goplus/spbase/mathf"
	"github.com/karetskiiVO/robotics/task2/solution/internal/robobpacket"
	"github.com/karetskiiVO/robotics/task2/solution/internal/transform"
	"github.com/phpgao/tlog"
)

var packageSize int = 4096

func (r *Robot) telemetryDemon() {
	buffer := make([]byte, packageSize)

	for {
		select {
		case <-r.ctx.Done():
			return
		default:
		}

		size, err := r.telemetryConn.Read(buffer[:packageSize])
		if err != nil {
			tlog.WarnContextf(r.ctx, "can't recieve packet: %v", err)
			continue
		}
		raw := buffer[:size]

		telemetry := robobpacket.Telemetry{}
		err = robobpacket.ParseTelemetry(raw, &telemetry)
		if err != nil {
			tlog.WarnContextf(r.ctx, "can't parse: %v data: %v", err, raw)
			continue
		}

		tlog.TraceContextf(r.ctx, "get package (.size=%v) on coords (.x=%.2f .y=%.2f)",
			telemetry.Header.Size,
			telemetry.Header.OdomX,
			telemetry.Header.OdomY,
		)

		// UPDATE
		r.nav.Update(&telemetry)
		// SEND TO FRONT

		// Корявый прототип
		trace, err := r.nav.TraceToTarget(transform.NewVec2(0, 3))
		if err != nil {
			tlog.ErrorContextf(r.ctx, "While calculate trace: %v", err)
			continue
		}

		// просто проехаться по прямой
		if len(trace) == 2 {
			x, y := trace[1].Sub(trace[0]).Unpak()
			r.target <- transform.Transform{Position: trace[1], Thetha: float32(mathf.Atan2(y, x))}
		} else {
			dir1, dir2 := trace[1].Sub(trace[0]), trace[2].Sub(trace[0])

			// коллинеарность
			if dir1.Cross(dir2) < 1e-3 || trace[1].Sub(trace[0]).Mag2() < 1e-3 {
				x, y := trace[2].Sub(trace[0]).Unpak()
				r.target <- transform.Transform{Position: trace[1], Thetha: float32(mathf.Atan2(y, x))}
			} else {
				x, y := trace[2].Sub(trace[0]).Unpak()
				r.target <- transform.Transform{Position: trace[1], Thetha: float32(mathf.Atan2(y, x))}
			}
		}

		// r.target <- ...
	}
}
