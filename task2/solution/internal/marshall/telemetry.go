package marshall

import (
	"bytes"
	"encoding/binary"
	"errors"
	"net"
	"log"
)

type TelemPacket struct {
	Header struct {
		Size uint32
		Magic uint32
		OdomX float32
		OdomY float32
		OdomTh float32
		V float32
		Vy float32
		Vth float32
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
	defer d.conn.Close()

	for {
		_, err := d.conn.Read(buffer[:headerSize])
		if err != nil {
			log.Println(err)
			return err
		}

		if err = unmarshallTelemetry(buffer[:headerSize], telem); err != nil {
			log.Println(err)
			return err
		}

		dataSize := telem.Header.LidarDataSize * 4
		_, err = d.conn.Read(buffer[:dataSize])
		if err != nil {
			log.Println(err)
			return err
		}

		binary.Decode(buffer[:dataSize], binary.LittleEndian, telem.LidarData)
		d.channel <- *telem
	}
}

// Внимание! Блокируется, пока не получит пакет телеметрии
func (d *Telemetry)Receive() *TelemPacket {
	packet := <- d.channel
	return &packet
}

func unmarshallTelemetry(raw []byte, telem *TelemPacket) error {
	reader := bytes.NewReader(raw)
	binary.Read(reader, binary.LittleEndian, &telem.Header)
	if telem.Header.Magic != magic {
		return errors.New("Invalid magic")
	}
	return nil
}
