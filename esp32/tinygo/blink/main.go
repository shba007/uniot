package main

import (
	"machine"
	"time"
)

func main() {
	// Configure serial (UART0) at 115200 baud.
	machine.Serial.Configure(machine.UARTConfig{BaudRate: 115200})

	// Configure GPIO22 as output.
	led := machine.LED
	led.Configure(machine.PinConfig{Mode: machine.PinOutput})

	for {
		led.High()
		println("+")
		time.Sleep(time.Second)

		led.Low()
		println("-")
		time.Sleep(time.Second)
	}
}