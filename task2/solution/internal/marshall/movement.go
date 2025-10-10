package marshall

import (
	"encoding/binary"
	"log"
	"net"
)

type State struct {
	V     float32
	Omega float32
}

type Commands struct {
	conn    net.Conn
	channel chan State
}

const txBufferSize int = 16

func (c *Commands) Setup(address string) error {
	conn, err := net.Dial("udp", address)
	c.channel = make(chan State)
	if err != nil {
		return err
	}
	c.conn = conn
	return nil
}

// TODO: если значение неправильное, не падать, а обрезать его по границе?
func (c *Commands) SetVelocity(v float64, omega float64) error {
	v32, omega32 := float32(v), float32(omega)
	state := State{V: v32, Omega: omega32}
	c.channel <- state
	return nil
}

func (c *Commands) Loop() error {
	buffer := make([]byte, txBufferSize)
	defer c.conn.Close()

	for {
		state := <-c.channel
		binary.Encode(buffer, binary.LittleEndian, state)
		_, err := c.conn.Write(buffer)
		if err != nil {
			log.Println(err)
			return err
		}
	}
}
