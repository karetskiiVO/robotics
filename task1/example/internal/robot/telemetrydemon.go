package robot

import (
	"log"

	"github.com/karetskiiVO/robotics/task1/example/internal/robobpacket"
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

		size, err := r.telemetryConn.Read(buffer[:0])
		if err != nil {
			log.Printf("%v recieve packet: %v", warnStr, err)
			continue
		}

		raw := buffer[:size]

		telemetry := robobpacket.Telemetry{}
		err = robobpacket.ParseTelemetry(raw, &telemetry)
		if err != nil {
			log.Printf("%v can't parse: %v data: %v", warnStr, err, raw)
			return
		}

		// UPDATE

		// SEND TO FRONT
	}
}
