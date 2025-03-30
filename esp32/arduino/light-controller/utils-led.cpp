#include "utils-led.h"

// Add global storage for named LEDs
static Led _leds[MAX_LEDS];
static uint8_t _ledCount = 0;

struct BlinkPattern {
  bool active;
  unsigned long interval;
  uint32_t cycles;  // 0 = infinite
  uint32_t cycleCount;
  unsigned long lastUpdate;
  bool state;
};

struct FadePattern {
  bool active;
  int startValue;
  int endValue;
  unsigned long duration;
  int cycles;  // 0 = infinite
  int currentStep;
  bool fadingUp;
  unsigned long lastUpdate;
  int totalSteps;
  unsigned long stepDelay;
};

static BlinkPattern blinkPattern[MAX_LEDS] = { 0 };
static FadePattern fadePattern[MAX_LEDS] = { 0 };


// Helper function to find LED index by name
static int findLedIndex(const String &ledName) {
  for (uint8_t i = 0; i < _ledCount; i++) {
    if (_leds[i].name == ledName) {
      return i;
    }
  }
  return -1;
}

void ledInit(const Led ledArray[], uint8_t count) {
  _ledCount = (count > MAX_LEDS) ? MAX_LEDS : count;
  for (uint8_t i = 0; i < _ledCount; i++) {
    _leds[i] = ledArray[i];
    pinMode(_leds[i].pin, OUTPUT);
    digitalWrite(_leds[i].pin, LOW);
  }
}

void ledOn(const String &ledName) {
  int idx = findLedIndex(ledName);
  if (idx != -1) {
    digitalWrite(_leds[idx].pin, HIGH);
  }
}

void ledOff(const String &ledName) {
  int idx = findLedIndex(ledName);
  if (idx != -1) {
    digitalWrite(_leds[idx].pin, LOW);
  }
}

void ledToggle(const String &ledName) {
  int idx = findLedIndex(ledName);
  if (idx != -1) {
    digitalWrite(_leds[idx].pin, !digitalRead(_leds[idx].pin));
  }
}

void ledBlink(const String &ledName, unsigned long interval, uint8_t cycles) {
  int idx = findLedIndex(ledName);
  if (idx != -1) {
    blinkPattern[idx].active = true;
    blinkPattern[idx].interval = interval;
    blinkPattern[idx].cycles = cycles;
    blinkPattern[idx].cycleCount = 0;
    blinkPattern[idx].lastUpdate = millis();
    blinkPattern[idx].state = LOW;
    digitalWrite(_leds[idx].pin, LOW);
  }
}


void ledFade(const String &ledName, int startValue, int endValue, unsigned long duration, int cycles) {
  int idx = findLedIndex(ledName);
  if (idx != -1) {
    fadePattern[idx].active = true;
    fadePattern[idx].startValue = startValue;
    fadePattern[idx].endValue = endValue;
    fadePattern[idx].duration = duration;
    fadePattern[idx].cycles = cycles;
    fadePattern[idx].totalSteps = 50;
    fadePattern[idx].stepDelay = duration / fadePattern[idx].totalSteps;
    fadePattern[idx].currentStep = 0;
    fadePattern[idx].fadingUp = true;
    fadePattern[idx].lastUpdate = millis();
    analogWrite(_leds[idx].pin, startValue);
  }
}

void updateLed() {
  unsigned long current = millis();
  for (int i = 0; i < _ledCount; i++) {
    // Update blink pattern if active
    if (blinkPattern[i].active) {
      if (current - blinkPattern[i].lastUpdate >= blinkPattern[i].interval) {
        blinkPattern[i].state = !blinkPattern[i].state;
        digitalWrite(_leds[i].pin, blinkPattern[i].state);
        blinkPattern[i].lastUpdate = current;
        if (!blinkPattern[i].state) {  // one full on-off cycle
          blinkPattern[i].cycleCount++;
          if (blinkPattern[i].cycles != 0 && blinkPattern[i].cycleCount >= blinkPattern[i].cycles) {
            blinkPattern[i].active = false;
          }
        }
      }
    }
    // Update fade pattern if active
    if (fadePattern[i].active) {
      if (current - fadePattern[i].lastUpdate >= fadePattern[i].stepDelay) {
        int startVal = fadePattern[i].startValue;
        int endVal = fadePattern[i].endValue;
        int delta = endVal - startVal;
        int totalSteps = fadePattern[i].totalSteps;
        int val = startVal + (delta * fadePattern[i].currentStep) / totalSteps;
        analogWrite(_leds[i].pin, val);
        fadePattern[i].lastUpdate = current;
        if (fadePattern[i].fadingUp) {
          fadePattern[i].currentStep++;
          if (fadePattern[i].currentStep > totalSteps) {
            fadePattern[i].fadingUp = false;
            fadePattern[i].currentStep = totalSteps - 1;
          }
        } else {
          fadePattern[i].currentStep--;
          if (fadePattern[i].currentStep < 0) {
            if (fadePattern[i].cycles != 0) {
              fadePattern[i].cycles--;
              if (fadePattern[i].cycles == 0) {
                fadePattern[i].active = false;
                continue;
              }
            }
            fadePattern[i].fadingUp = true;
            fadePattern[i].currentStep = 1;
          }
        }
      }
    }
  }
}
