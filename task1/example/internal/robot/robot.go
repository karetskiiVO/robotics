package robot

import (
	"context"
	"fmt"
	"net"
	"os"
	"sync"

	"github.com/karetskiiVO/robotics/task1/example/internal/transform"
	"github.com/phpgao/tlog"
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

	tlog.InfoContext(r.ctx, "Get env settings")
	telemetryHost := getEnvWithDefaulContext(r.ctx, "TEL_HOST", "localhost")
	telemetryPort := getEnvWithDefaulContext(r.ctx, "TEL_PORT", "5600")
	commandHost := getEnvWithDefaulContext(r.ctx, "CMD_HOST", "localhost")
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

	tlog.Warnf("Not found env with key %q used defult %q", envkey, defaultVal)
	return defaultVal
}

func getEnvWithDefaulContext(ctx context.Context, envkey, defaultVal string) string {
	res, ok := os.LookupEnv(envkey)
	if ok {
		return res
	}

	tlog.WarnContextf(ctx, "Not found env with key %q used defult %q", envkey, defaultVal)
	return defaultVal
}
