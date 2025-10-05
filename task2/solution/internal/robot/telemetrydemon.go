package robot

import (
	"github.com/karetskiiVO/robotics/task2/solution/internal/robobpacket"
	"github.com/karetskiiVO/robotics/task2/solution/internal/transform"
	"github.com/phpgao/tlog"
)

var packageSize int = 4096

func telemetryDemon(r *Robot) {
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
			tlog.ErrorContext(r.ctx, err)
		}
		_ = trace
	}
}
