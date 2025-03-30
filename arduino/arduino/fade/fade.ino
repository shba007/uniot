#define LED_PWM 9

int brightness = 0;  // how bright the LED is
int fadeAmount = 5;  // how many points to fade the LED by

void setup() {
  pinMode(LED_PWM, OUTPUT);
  Serial.begin(115200);  // initialize serial communication at 115200 baud
}

void loop() {
  // set the brightness of the LED
  analogWrite(LED_PWM, brightness);

  // print the current brightness value to the serial monitor
  Serial.println(brightness);

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  
  // wait for 30 milliseconds to see the dimming effect
  delay(30);
}
