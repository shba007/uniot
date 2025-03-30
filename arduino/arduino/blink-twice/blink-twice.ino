#define LED1 13
#define LED2 9

struct BlinkingLED {
  uint8_t id;
  uint8_t pin;
  unsigned long interval;
  unsigned long previousMillis;
  bool state;
};

BlinkingLED led1 = {1, LED1, 1000, 0, false};
BlinkingLED led2 = {2, LED2, 420, 0, false};

void blink(BlinkingLED &led) {
  unsigned long currentMillis = millis();
  if (currentMillis - led.previousMillis >= led.interval) {
    led.previousMillis = currentMillis;
    led.state = !led.state;
    digitalWrite(led.pin, led.state);
    
    // Print spaces based on the LED id (id - 1 spaces)
    for (uint8_t i = 0; i < led.id - 1; i++) {
      Serial.print(" ");
    }
    Serial.println(led.state ? "+" : "-");
  }
}

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  blink(led1);
  blink(led2);
}