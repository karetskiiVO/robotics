package movement

import (
	"net";
	"encoding/binary";
)

type State struct {
	V float32
	Omega float32
}

type Controller struct {
	Conn net.Conn
	buffer []byte
	State State
}

const bufferSize int = 16

func NewController() *Controller {
	c := new(Controller)
	c.buffer = make([]byte, bufferSize)
	return c
}

func (c Controller)Connect(address string) error {
	conn, err := net.Dial("udp", address)
	if err != nil {
		return err
	}
	c.Conn = conn
	return nil
}

func (c Controller)Update() error {
	binary.Encode(c.buffer, binary.LittleEndian, &c.State)	
	_, err := c.Conn.Write(c.buffer)
	if err != nil {
		return err
	}
	return nil
}
