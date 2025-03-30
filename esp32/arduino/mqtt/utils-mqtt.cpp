#include "utils-mqtt.h"

WiFiClientSecure wifiClient;
MqttClient mqttClient(wifiClient);

void connectMQTT(const char* broker, int port, const char* username, const char* password, const char* clientId) {
  wifiClient.setInsecure();
  mqttClient.setId(clientId);
  mqttClient.setUsernamePassword(username, password);

  Serial.print("Connecting to MQTT broker " + String(broker) + ":" + String(port));
  int retry = 0;
  while (!mqttClient.connect(broker, port) && retry < MQTT_MAX_RETRIES) {
    Serial.print(".");
    retry++;
    delay(1000);
  }
  if (mqttClient.connected()) {
    Serial.println("\nConnected to MQTT broker " + String(broker) + ":" + String(port));
  } else {
    Serial.print("\nMQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    while (1)
      ;
  }
}

void pollMQTT() {
  mqttClient.poll();
}

void publishMQTT(const char* topic, const char* payload) {
  Serial.print("Publishing message to ");
  Serial.println(topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(payload);
  mqttClient.endMessage();
}

struct Subscription {
  String topic;
  void (*callback)(char*);
};

Subscription subscriptions[MAX_SUBSCRIPTIONS];
int subCount = 0;

static void mqttMessageWrapper(int messageSize) {
  String topicStr = mqttClient.messageTopic();
  char* message = new char[messageSize + 1];
  for (int i = 0; i < messageSize; i++) {
    message[i] = (char)mqttClient.read();
  }
  message[messageSize] = '\0';

  for (int i = 0; i < subCount; i++) {
    if (subscriptions[i].topic == topicStr) {
      subscriptions[i].callback(message);
    }
  }
  delete[] message;
}

void subscribeMQTT(const char* topic, void (*callback)(char*)) {
  // Prevent duplicate subscriptions for the same topic
  for (int i = 0; i < subCount; i++) {
    if (subscriptions[i].topic == topic) {
      subscriptions[i].callback = callback; // Update callback if needed
      return;
    }
  }
  
  if (subCount < MAX_SUBSCRIPTIONS) {
    subscriptions[subCount].topic = topic;
    subscriptions[subCount].callback = callback;
    subCount++;
  }
  mqttClient.subscribe(topic);
  // Set onMessage callback only once
  static bool onMessageSet = false;
  if (!onMessageSet) {
    mqttClient.onMessage(mqttMessageWrapper);
    onMessageSet = true;
  }
}
