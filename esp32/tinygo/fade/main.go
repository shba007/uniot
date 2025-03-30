package main

import (
	"machine"
	"time"
)

// pwmCycle simulates one PWM period.
// It turns the LED on for a duration proportional to brightness (0-255),
// then off for the remainder of the period.
func pwmCycle(pin machine.Pin, brightness uint8, period time.Duration) {
	onTime := time.Duration(brightness) * period / 255
	offTime := period - onTime

	if onTime > 0 {
		pin.High()
		time.Sleep(onTime)
	}
	if offTime > 0 {
		pin.Low()
		time.Sleep(offTime)
	}
}

func main() {
	// Configure serial (UART0) at 115200 baud for debugging.
	uart := machine.Serial
	uart.Configure(machine.UARTConfig{BaudRate: 115200})

	led_pwm := 4

	// Configure pin 22 as the PWM output for the LED.
	led := machine.Pin(led_pwm)
	led.Configure(machine.PinConfig{Mode: machine.PinOutput})

	// Fade parameters.
	var brightness uint8 = 0 // LED brightness (0-255)
	var fadeAmount int8 = 5  // Step to change brightness

	// We'll simulate a 30ms update period by running several PWM cycles.
	pwmPeriod := 10 * time.Millisecond // one PWM cycle period
	cycles := 3                        // 3 cycles x 10ms = 30ms total

	for {
		// Run multiple PWM cycles to simulate analogWrite over 30ms.
		for i := 0; i < cycles; i++ {
			pwmCycle(led, brightness, pwmPeriod)
		}

		// print the current brightness value to the serial monitor
		println(brightness)

		// Update brightness.
		// Convert brightness to int for arithmetic.
		b := int(brightness) + int(fadeAmount)
		if b < 0 {
			b = 0
		} else if b > 255 {
			b = 255
		}
		brightness = uint8(b)

		// Reverse fade direction at the ends.
		if brightness == 0 || brightness == 255 {
			fadeAmount = -fadeAmount
		}
	}
}
