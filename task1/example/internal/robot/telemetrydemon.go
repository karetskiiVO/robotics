package robot

import (
	"github.com/karetskiiVO/robotics/task1/example/internal/robobpacket"
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

		tlog.DebugContextf(r.ctx, "get package (.size=%v) on coords (.x=%.2f .y=%.2f)",
			telemetry.Header.Size,
			telemetry.Header.OdomX,
			telemetry.Header.OdomY,
		)

		// UPDATE

		// SEND TO FRONT
	}
}
