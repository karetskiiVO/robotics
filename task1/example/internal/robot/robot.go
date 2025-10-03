package robot

import (
	"context"
	"fmt"
	"log"
	"net"
	"os"
	"sync"

	"github.com/karetskiiVO/robotics/task1/example/internal/transform"
)

type Robot struct {
	telemetryListner net.Listener

	telemetryConn net.Conn
	commandConn   net.Conn

	// frontend

	ctx    context.Context
	cancel func()

	transformMU sync.RWMutex
	transform   transform.Transform
}

func NewRobot(ctx context.Context) (*Robot, error) {
	var err error
	r := &Robot{}
	r.ctx, r.cancel = context.WithCancel(ctx)

	log.Printf("Get env settings")
	telemetryHost := getEnvWithDefaul("TEL_HOST", "localhost")
	telemetryPort := getEnvWithDefaul("TEL_PORT", "5600")
	commandHost := getEnvWithDefaul("CMD_HOST", "localhost")
	commandPort := getEnvWithDefaul("CMD_PORT", "5555")

	commandAddr := fmt.Sprintf("%v:%v", commandHost, commandPort)
	telemetryAddr := fmt.Sprintf("%v:%v", telemetryHost, telemetryPort)
	log.Printf("TEL_HOST: %q TEL_PORT: %q CMD_HOST: %q CMD_PORT: %q",
		telemetryHost, telemetryPort,
		telemetryHost, telemetryPort,
	)

	log.Printf("Parse IP addresses [telemetry:%q command:%q]", telemetryAddr, commandAddr)
	r.telemetryListner, err = net.Listen("tcp", telemetryAddr)
	if err != nil {
		return nil, fmt.Errorf("listen telemetry port: %w", err)
	}
	r.commandConn, err = net.Dial("udp", commandAddr)
	if err != nil {
		return nil, fmt.Errorf("dial comand port: %w", err)
	}

	log.Printf("Accept robot telemetry")
	r.telemetryConn, err = r.telemetryListner.Accept()
	if err != nil {
		return nil, fmt.Errorf("accept telemetry connection: %w", err)
	}

	log.Printf("Start net demons")
	go telemetryDemon(r)

	return r, nil
}

func (r *Robot) Run() error {
	for {
	}
}

func (r *Robot) Dispose() {
	r.cancel()
	r.telemetryListner.Close()
	r.telemetryConn.Close()
	r.commandConn.Close()
}

func getEnvWithDefaul(envkey, defaultVal string) string {
	res, ok := os.LookupEnv(envkey)
	if ok {
		return res
	}

	// WARN
	return defaultVal
}
