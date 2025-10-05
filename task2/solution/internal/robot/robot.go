package robot

import (
	"context"
	"fmt"
	"net"

	"github.com/karetskiiVO/robotics/task2/solution/internal/navigation"
	"github.com/karetskiiVO/robotics/task2/solution/internal/transform"
	"github.com/phpgao/tlog"
)

type Robot struct {
	telemetryListner net.Listener

	telemetryConn net.Conn
	commandConn   net.Conn

	// frontend

	ctx    context.Context
	cancel func()

	nav    *navigation.System
	target chan transform.Transform
}

func NewRobot(ctx context.Context) (*Robot, error) {
	var err error
	r := &Robot{}
	r.ctx, r.cancel = context.WithCancel(ctx)

	r.nav = navigation.New(ctx, -10, -10, 10, 10, float32(10)/float32(1<<8))

	tlog.InfoContext(r.ctx, "Get env settings")
	telemetryHost := getEnvWithDefaulContext(r.ctx, "TEL_HOST", "127.0.0.1")
	telemetryPort := getEnvWithDefaulContext(r.ctx, "TEL_PORT", "5600")
	commandHost := getEnvWithDefaulContext(r.ctx, "CMD_HOST", "127.0.0.1")
	commandPort := getEnvWithDefaulContext(r.ctx, "CMD_PORT", "5555")

	commandAddr := fmt.Sprintf("%v:%v", commandHost, commandPort)
	telemetryAddr := fmt.Sprintf("%v:%v", telemetryHost, telemetryPort)
	tlog.InfoContextf(r.ctx, "TEL_HOST: %q TEL_PORT: %q CMD_HOST: %q CMD_PORT: %q",
		telemetryHost, telemetryPort,
		telemetryHost, telemetryPort,
	)

	tlog.InfoContextf(r.ctx, "Parse IP addresses [telemetry:%q command:%q]", telemetryAddr, commandAddr)
	r.telemetryListner, err = net.Listen("tcp", telemetryAddr)
	if err != nil {
		return nil, fmt.Errorf("listen telemetry port: %w", err)
	}
	r.commandConn, err = net.Dial("udp", commandAddr)
	if err != nil {
		return nil, fmt.Errorf("dial comand port: %w", err)
	}

	tlog.InfoContext(r.ctx, "Accept robot telemetry")
	r.telemetryConn, err = r.telemetryListner.Accept()
	if err != nil {
		return nil, fmt.Errorf("accept telemetry connection: %w", err)
	}

	tlog.InfoContext(r.ctx, "Start net demons")
	r.target = make(chan transform.Transform, 1)

	go r.telemetryDemon()

	return r, nil
}

func (r *Robot) Run() error {
	r.commandDemon()

	return nil
}

func (r *Robot) Dispose() {
	r.cancel()
	r.telemetryListner.Close()
	r.telemetryConn.Close()
	r.commandConn.Close()
}
