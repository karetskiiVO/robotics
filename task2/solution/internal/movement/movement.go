package movement

import (
	"net";
	"encoding/binary";
	"errors";
)

type State struct {
	V float32
	Omega float32
}

type Controller struct {
	conn net.Conn
	buffer []byte
}

const bufferSize int = 16
const MaxV float32 = 0.5
const MaxOmega float32 = 1.0

func NewController() *Controller {
	c := new(Controller)
	c.buffer = make([]byte, bufferSize)
	return c
}

func (c *Controller)Connect(address string) error {
	conn, err := net.Dial("udp", address)
	if err != nil {
		return err
	}
	c.conn = conn
	return nil
}

func (c *Controller)SetState(v float32, omega float32) error {
	if v > MaxV || omega > MaxOmega {
		return errors.New("Invalid parameters")
	}
	state := State{V: v, Omega: omega}
	binary.Encode(c.buffer, binary.LittleEndian, state)	
	_, err := c.conn.Write(c.buffer)
	if err != nil {
		return err
	}
	return nil
}

func (c *Controller)Disconnect() error {
	if c.conn != nil {
		return c.conn.Close()
	}
	return nil
}
