package marshall

import (
	"net";
	"encoding/binary";
	"errors";
	"fmt"
)

type State struct {
	V float32
	Omega float32
}

type Commands struct {
	conn net.Conn
	channel chan State
}

const txBufferSize int = 16
const MaxV float32 = 0.5
const MaxOmega float32 = 1.0
const MaxAcc float32 = 0.05

func (c *Commands)Setup(address string) error {
	conn, err := net.Dial("udp", address)
	c.channel = make(chan State)
	if err != nil {
		return err
	}
	c.conn = conn
	return nil
}

func (c *Commands) SetVelocity(v float32, omega float32) error {
	if v > MaxV || omega > MaxOmega {
		return errors.New("Invalid parameters")
	}
	state := State{V: v, Omega: omega}	
	c.channel <- state
	return nil
}

func (c *Commands)Loop() error {
	buffer := make([]byte, txBufferSize)
	fmt.Println("Started")
	for {
		state := <- c.channel		
		binary.Encode(buffer, binary.LittleEndian, state)	
		_, err := c.conn.Write(buffer)
		if err != nil {
			return err
		}
	}
}

func (c *Commands)Dispose() error {
	if c.conn != nil {
		return c.conn.Close()
	}
	return nil
}
