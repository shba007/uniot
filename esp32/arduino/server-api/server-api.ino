#include <WebServer.h>
#include <ArduinoJson.h>
#include "utils-led.h"
#include "utils-wifi.h"

// WiFi credentials
const char targetSSID[] = "SKYNET";
const char targetPassword[] = "thehacker@678934";

// Define LED array with names and corresponding pins
Led ledArray[] = {
  { "RED", 2 },
  { "BLUE", 4 },
  { "GREEN", 5 }
};
const uint8_t ledCount = sizeof(ledArray) / sizeof(ledArray[0]);

// Create a web server on port 80
WebServer server(8080);

// REST API endpoint handler: expects /led?cmd=<LEDNAME:COMMAND>
// Supports ON, OFF, TOGGLE, BLINK,<interval>,<cycles> and FADE,<startValue>,<endValue>,<duration>,<cycles>

void ledControlRestHandler() {
  String cmd;
  if (server.method() == HTTP_GET) {
    if (!server.hasArg("cmd")) {
      Serial.println("[LOG] Missing cmd parameter");
      server.send(400, "application/json", "{\"error\":\"Missing cmd parameter\"}");
      return;
    }
    cmd = server.arg("cmd");
    Serial.println("[LOG] Received GET command: " + cmd);
  } else if (server.method() == HTTP_POST) {
    String body = server.arg("plain");
    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, body);
    if (error) {
      Serial.println("[LOG] Failed to parse JSON: " + String(error.c_str()));
      server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      return;
    }
    if (!doc.containsKey("cmd")) {
      Serial.println("[LOG] JSON does not contain cmd key");
      server.send(400, "application/json", "{\"error\":\"Missing cmd key in JSON\"}");
      return;
    }
    cmd = doc["cmd"].as<String>();
    Serial.println("[LOG] Received POST JSON command: " + cmd);
  } else {
    server.send(405, "application/json", "{\"error\":\"Method Not Allowed\"}");
    return;
  }

  char buffer[cmd.length() + 1];
  cmd.toCharArray(buffer, cmd.length() + 1);
  String msg = String(buffer);
  int delimIndex = msg.indexOf(':');
  if (delimIndex == -1) {
    Serial.println("[LOG] Invalid command format. Expected LEDNAME:COMMAND");
    server.send(400, "application/json", "{\"error\":\"Invalid command format\"}");
    return;
  }

  String ledName = msg.substring(0, delimIndex);
  String command = msg.substring(delimIndex + 1);
  ledName.trim();
  command.trim();
  Serial.println("[LOG] Processing LED: " + ledName + " | Command: " + command);

  if (command.equalsIgnoreCase("ON")) {
    ledOn(ledName);
    Serial.println("[LOG] LED " + ledName + " turned ON");
  } else if (command.equalsIgnoreCase("OFF")) {
    ledOff(ledName);
    Serial.println("[LOG] LED " + ledName + " turned OFF");
  } else if (command.equalsIgnoreCase("TOGGLE")) {
    ledToggle(ledName);
    Serial.println("[LOG] LED " + ledName + " toggled");
  } else if (command.startsWith("BLINK")) {
    int firstComma = command.indexOf(',');
    int secondComma = command.indexOf(',', firstComma + 1);
    if (firstComma == -1 || secondComma == -1) {
      Serial.println("[LOG] Invalid BLINK command format. Expected BLINK,<interval>,<cycles>");
      server.send(400, "application/json", "{\"error\":\"Invalid BLINK command format\"}");
      return;
    }
    unsigned long interval = command.substring(firstComma + 1, secondComma).toInt();
    uint8_t cycles = command.substring(secondComma + 1).toInt();
    ledBlink(ledName, interval, cycles);
    Serial.println("[LOG] LED " + ledName + " set to BLINK with interval " + String(interval) + " and cycles " + String(cycles));
  } else if (command.startsWith("FADE")) {
    int firstComma = command.indexOf(',');
    int secondComma = command.indexOf(',', firstComma + 1);
    int thirdComma = command.indexOf(',', secondComma + 1);
    int fourthComma = command.indexOf(',', thirdComma + 1);
    if (firstComma == -1 || secondComma == -1 || thirdComma == -1 || fourthComma == -1) {
      Serial.println("[LOG] Invalid FADE command format. Expected FADE,<startValue>,<endValue>,<duration>,<cycles>");
      server.send(400, "application/json", "{\"error\":\"Invalid FADE command format\"}");
      return;
    }
    int startValue = command.substring(firstComma + 1, secondComma).toInt();
    int endValue = command.substring(secondComma + 1, thirdComma).toInt();
    unsigned long duration = command.substring(thirdComma + 1, fourthComma).toInt();
    int cycles = command.substring(fourthComma + 1).toInt();
    ledFade(ledName, startValue, endValue, duration, cycles);
    Serial.println("[LOG] LED " + ledName + " set to FADE from " + String(startValue) + " to " + String(endValue) + " over " + String(duration) + "ms for " + String(cycles) + " cycles");
  } else {
    Serial.println("[LOG] Unknown command: " + command);
    server.send(400, "application/json", "{\"error\":\"Unknown command\"}");
    return;
  }

  server.send(200, "application/json", "{\"status\":\"OK\"}");
}

void setup() {
  Serial.begin(115200);
  delay(100);

  ledInit(ledArray, ledCount);
  Serial.println("LEDs initialized.");

  connectWifi(targetSSID, targetPassword);

  server.on("/led", HTTP_POST, ledControlRestHandler);
  server.begin();
  Serial.println("HTTP server started.");
}

void loop() {
  if (!isWifiConnected()) {
    Serial.println("\nWiFi disconnected. Reconnecting...");
    connectWifi(targetSSID, targetPassword);

    server.on("/led", HTTP_POST, ledControlRestHandler);
    server.begin();
    Serial.println("HTTP server started.");
  }

  server.handleClient();
  updateLed();
}