package telemetry

import (
	"encoding/binary"
	"errors"
	"net"
)

type Telemetry struct {
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

const headerSize int = 12
const bufferSize int = 2048
const magic uint32 = 1196704343

type Demon struct {
	conn net.Conn
	telem Telemetry
}

func SummonDemon() *Demon {
	demon := new(Demon)
	demon.buffer = make([]byte, bufferSize)
	return demon
}

func (d *Demon) Ponder(address string) error {
	list, err := net.Listen("tcp", address)
	if err != nil {
		return err
	}

	d.conn, err = list.Accept()
	if err != nil {
		return err
	}
	return nil	
}

func (d *Demon) Posess() error {
	buffer := make([]byte, bufferSize)
	for {
		_, err := d.conn.Read(buffer[:headerSize])
		if err != nil {
			return err
		}

		if err = d.unmarshallTelemetry(buffer); err != nil {
			return err
		}
	}
}

func (d *Demon)unmarshallTelemetry(raw []byte) error {
	binary.Decode(raw, binary.LittleEndian, &d.telem)
	if d.telem.Header.Magic != magic {
		return errors.New("Invalid magic")
	}
	return nil
}
