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

unsigned long interval = 5000;
unsigned long previousMillis = 0;

void messageHandler1(char* message) {
  Serial.println("Listening message to messageHandler1 " + String(message));
}

void messageHandler2(char* message) {
  Serial.println("Listening message to messageHandler2 " + String(message));
}

void setup() {
  Serial.begin(115200);
  connectWifi(targetSSID, targetPassword);
  
  timeinfo = updateLocalTime();
  String gmtime = String(formatGMTTime(timeinfo, 0));
  String localtime = String(formatGMTTime(timeinfo, 19800));
  Serial.println("GM Time: " + gmtime + " | Local Time: " + localtime);

  connectMQTT(mqttServer, mqttPort, mqttUsername, mqttPassword, mqttClientId);
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
  }

  pollMQTT();

  subscribeMQTT("test/topic1", messageHandler1);
  subscribeMQTT("test/topic2", messageHandler2);

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    publishMQTT("test/topic", "Hello world");
  }
}