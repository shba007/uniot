#include "utils-led.h"
#include "utils-wifi.h"
#include "utils-mqtt.h"
#include "utils-ntp.h"

const char targetSSID[] = "SKYNET";
const char targetPassword[] = "thehacker@678934";

struct tm* timeinfo;

const char mqttServer[] = "049e7c2da9a34433b3401f5187b199c1.s1.eu.hivemq.cloud";
const int mqttPort = 8883;
const char mqttUsername[] = "IoTDevice";
const char mqttPassword[] = "IoTDevice@100";
const char mqttClientId[] = "IoT@1";

// Define LED array with names and corresponding pins
Led allLeds[] = {
  { "RED", 2 },
  { "BLUE", 4 },
  { "GREEN", 5 }
};
const uint8_t ledCount = sizeof(allLeds) / sizeof(allLeds[0]);

void ledControlHandler(char* message) {
  String msg = String(message);
  int delimIndex = msg.indexOf(':');
  if (delimIndex == -1) return;  // Invalid format, expected "LEDNAME:COMMAND"

  String ledName = msg.substring(0, delimIndex);
  String command = msg.substring(delimIndex + 1);
  ledName.trim();
  command.trim();

  Serial.println("Received control command - LED: " + ledName + " | Command: " + command);

  if (command.equalsIgnoreCase("ON")) {
    ledOn(ledName);
  } else if (command.equalsIgnoreCase("OFF")) {
    ledOff(ledName);
  } else if (command.equalsIgnoreCase("TOGGLE")) {
    ledToggle(ledName);
  } else if (command.startsWith("BLINK")) {
    // Expected format: BLINK,<interval>,<cycles>
    int firstComma = command.indexOf(',');
    int secondComma = command.indexOf(',', firstComma + 1);
    if (firstComma == -1 || secondComma == -1) {
      Serial.println("Invalid BLINK command format. Expected BLINK,<interval>,<cycles>");
      return;
    }
    unsigned long interval = command.substring(firstComma + 1, secondComma).toInt();
    uint8_t cycles = command.substring(secondComma + 1).toInt();
    ledBlink(ledName, interval, cycles);
  } else if (command.startsWith("FADE")) {
    // Expected format: FADE,<startValue>,<endValue>,<duration>,<cycles>
    int firstComma = command.indexOf(',');
    int secondComma = command.indexOf(',', firstComma + 1);
    int thirdComma = command.indexOf(',', secondComma + 1);
    int fourthComma = command.indexOf(',', thirdComma + 1);
    if (firstComma == -1 || secondComma == -1 || thirdComma == -1 || fourthComma == -1) {
      Serial.println("Invalid FADE command format. Expected FADE,<startValue>,<endValue>,<duration>,<cycles>");
      return;
    }
    int startValue = command.substring(firstComma + 1, secondComma).toInt();
    int endValue = command.substring(secondComma + 1, thirdComma).toInt();
    unsigned long duration = command.substring(thirdComma + 1, fourthComma).toInt();
    int cycles = command.substring(fourthComma + 1).toInt();
    ledFade(ledName, startValue, endValue, duration, cycles);
  }
}

void setup() {
  Serial.begin(115200);

  ledInit(allLeds, ledCount);
  connectWifi(targetSSID, targetPassword);

  timeinfo = updateLocalTime();
  String gmtime = String(formatGMTTime(timeinfo, 0));
  String localtime = String(formatGMTTime(timeinfo, 19800));
  Serial.println("GM Time: " + gmtime + " | Local Time: " + localtime);

  connectMQTT(mqttServer, mqttPort, mqttUsername, mqttPassword, mqttClientId);
  subscribeMQTT("led/control", ledControlHandler);
}

void loop() {
  if (!isWifiConnected()) {
    Serial.println("\nWiFi disconnected. Reconnecting...");
    connectWifi(targetSSID, targetPassword);

    timeinfo = updateLocalTime();
    String gmtime = String(formatGMTTime(timeinfo, 0));
    String localtime = String(formatGMTTime(timeinfo, 19800));
    Serial.println("GM Time: " + gmtime + " | Local Time: " + localtime);

    connectMQTT(mqttServer, mqttPort, mqttUsername, mqttPassword, mqttClientId);
    subscribeMQTT("led/control", ledControlHandler);
  }

  updateLed();
  pollMQTT();
}
