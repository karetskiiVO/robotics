package robobpacket

import (
	"bytes"
	"encoding/binary"
	"fmt"
)

//go:packed
type Telemetry struct {
	Header struct {
		Size   uint32
		magic  [4]byte
		OdomX  float32
		OdomY  float32
		OdomTh float32
		VX     float32
		VY     float32
		VTh    float32
		Wx     float32
		Wy     float32
		Wz     float32
	}
	Lidar []float32
}

var magic = []byte("WBTG")

func ParseTelemetry(data []byte, t *Telemetry) error {
	r := bytes.NewReader(data)

	var err error
	err = binary.Read(r, binary.LittleEndian, &t.Header)
	if err != nil {
		return fmt.Errorf("binary header reading: %w", err)
	}

	lidarLen := t.Header.Size/4 - 11
	t.Lidar = make([]float32, lidarLen)

	err = binary.Read(r, binary.LittleEndian, &t.Lidar)
	if err != nil {
		return fmt.Errorf("binary lidar reading: %w", err)
	}

	return t.Validate()
}

func (t Telemetry) Validate() error {
	if !bytes.Equal(t.Header.magic[:], magic) {
		return fmt.Errorf("wrong magic expected: %v get: %v", magic, t.Header.magic[:])
	}

	return nil
}
