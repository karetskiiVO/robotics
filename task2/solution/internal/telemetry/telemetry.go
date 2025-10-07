package telemetry

import (
	"bytes"
	"encoding/binary"
	"errors"
	"net"
	"sync"
	"fmt"
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

const headerSize int = 12 * 8
const bufferSize int = 2048
const magic uint32 = 1196704343

type Demon struct {
	conn net.Conn
	telem Telemetry
	mu sync.RWMutex
}

func SummonDemon() *Demon {
	demon := new(Demon)
	return demon
}

func (d *Demon) Posess(address string) error {
	list, err := net.Listen("tcp", address)
	if err != nil {
		return err
	}

	d.conn, err = list.Accept()
	if err != nil {
		return err
	}
	defer d.conn.Close()

	buffer := make([]byte, bufferSize)
	for {
		_, err := d.conn.Read(buffer[:headerSize])
		if err != nil {
			return err
		}

		d.mu.Lock()
		if err = d.unmarshallTelemetry(buffer[:headerSize]); err != nil {
			fmt.Printf("error\n")
			return err
		}
		d.mu.Unlock()

		dataSize := d.telem.Header.LidarDataSize * 4
		_, err = d.conn.Read(buffer[:dataSize])
		if err != nil {
			return err
		}

		d.mu.Lock()
		binary.Decode(buffer[:dataSize], binary.LittleEndian, d.telem.LidarData)
		d.mu.Unlock()
	}
}

func (d *Demon)Telemetry() *Telemetry {
	d.mu.RLock()
	defer d.mu.RUnlock()

	return &d.telem
}

func (d *Demon)unmarshallTelemetry(raw []byte) error {
	reader := bytes.NewReader(raw)
	binary.Read(reader, binary.LittleEndian, &d.telem.Header)
	if d.telem.Header.Magic != magic {
		return errors.New("Invalid magic")
	}
	return nil
}
