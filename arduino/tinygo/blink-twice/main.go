package main

import (
	"machine"
	"time"
)

func blink(id int, pin int, interval int) {
	// Precompute prefix: (id - 1) spaces.
	prefix := ""
	for i := 0; i < id-1; i++ {
		prefix += " "
	}
	led := machine.Pin(pin)
	led.Configure(machine.PinConfig{Mode: machine.PinOutput})
	for {
		println(prefix + "+")
		led.Low()
		time.Sleep(time.Millisecond * time.Duration(interval))

		println(prefix + "-")
		led.High()
		time.Sleep(time.Millisecond * time.Duration(interval))
	}
}

func main() {
	// Configure serial (UART0) at 115200 baud.
	machine.Serial.Configure(machine.UARTConfig{BaudRate: 115200})

	// Run blink functions concurrently.
	go blink(1, 13, 1000)
	blink(2, 9, 420)
}
