#include <Arduino.h>
#include <Servo.h>

Servo myServo;

int ServoPin = 11;
int angle = 0;
int delta = 10;

void setup() {
  Serial.begin(115200);
  myServo.attach(ServoPin);
  delay(15);  // Delay to allow servo to move to initial position
}

void loop() {
  // 0 -> 180
  // 180 -> 30
  // 30 -> 180
  // 180 -> 60
  for (int cycle = 0; cycle <= 180/delta; cycle += 1) {
    for (angle = cycle * delta; angle <= 180; angle += 1) {
      myServo.write(angle);
      // Serial.printf("Servo Motor angle %d degree", angle);
      Serial.println();
      delay(10);
    }
    for (angle = 180; angle >= (cycle + 1) * delta; angle -= 1) {
      myServo.write(angle);
      // Serial.printf("Servo Motor angle %d degree", angle);
      Serial.println();
      delay(10);
    }
  }
}