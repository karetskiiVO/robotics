package marshall

import (
	"bytes"
	"encoding/binary"
	"errors"
	"net"
	"fmt"
)
var _ = fmt.Printf

type TelemPacket struct {
	Header struct {
		Size uint32
		Magic uint32
		OdomX float32
		OdomY float32
		OdomTh float32
		Vx float32
		Vy float32
		Vz float32
		Wx float32
		Wy float32
		Wz float32
		LidarDataSize uint32
	}
	LidarData []float32
}

const headerSize int = 12 * 8
const rxBufferSize int = 2048
const magic uint32 = 1196704343

type Telemetry struct {
	conn net.Conn
	channel chan TelemPacket
}

func (d *Telemetry) Setup(address string) error {
	list, err := net.Listen("tcp", address)
	if err != nil {
		return err
	}

	d.conn, err = list.Accept()
	if err != nil {
		return err
	}
	d.channel = make(chan TelemPacket)
	return nil
}

func (d *Telemetry) Loop() error {
	buffer := make([]byte, rxBufferSize)
	telem := new(TelemPacket)
	for {
		_, err := d.conn.Read(buffer[:headerSize])
		if err != nil {
			return err
		}

		if err = d.unmarshallTelemetry(buffer[:headerSize], telem); err != nil {
			return err
		}

		dataSize := telem.Header.LidarDataSize * 4
		_, err = d.conn.Read(buffer[:dataSize])
		if err != nil {
			return err
		}

		binary.Decode(buffer[:dataSize], binary.LittleEndian, telem.LidarData)
		d.channel <- *telem
	}
}

func (d *Telemetry) Dispose() error {
	return d.conn.Close()
}

// Внимание! Блокируется, пока не получит пакет телеметрии
func (d *Telemetry)Receive() *TelemPacket {
	packet := <- d.channel
	return &packet
}

func (d *Telemetry)unmarshallTelemetry(raw []byte, telem *TelemPacket) error {
	reader := bytes.NewReader(raw)
	binary.Read(reader, binary.LittleEndian, telem.Header)
	if telem.Header.Magic != magic {
		return errors.New("Invalid magic")
	}
	return nil
}
