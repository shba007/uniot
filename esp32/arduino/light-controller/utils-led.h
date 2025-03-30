#ifndef UTILS_LED_H
#define UTILS_LED_H

#include <Arduino.h>

#define MAX_LEDS 10

// Add struct for LED with a name and pin number
struct Led {
  String name;
  uint8_t pin;
};

// New function prototypes for named LED control and patterns
void ledInit(const Led ledArray[], uint8_t count);
void ledOn(const String &ledName);
void ledOff(const String &ledName);
void ledToggle(const String &ledName);
void ledBlink(const String &ledName, unsigned long interval, uint8_t cycles);
void ledFade(const String &ledName, int startValue, int endValue, unsigned long duration, int cycles);
void updateLed();

#endif  // UTILS_LED_H
