#ifndef UTILS_MQTT_H
#define UTILS_MQTT_H

#include <WiFiClientSecure.h>
#include <ArduinoMqttClient.h>

extern WiFiClientSecure wifiClient;
extern MqttClient mqttClient;

#define MQTT_MAX_RETRIES 5
#define MAX_SUBSCRIPTIONS 20

void connectMQTT(const char* broker, int port, const char* username, const char* password, const char* clientId);
void pollMQTT();
void publishMQTT(const char* topic, const char* payload);
void subscribeMQTT(const char* topic, void (*callback)(char*));

#endif  // UTILS_MQTT_H
