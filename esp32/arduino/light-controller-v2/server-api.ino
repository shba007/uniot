#include <WebServer.h>
#include "utils-wifi.h"

// WiFi credentials
const char targetSSID[] = "SKYNET";
const char targetPassword[] = "thehacker@678934";

// Create a web server on port 8080
WebServer server(8080);

// REST API endpoint handler for Hello World
void handleHealth() {
  server.send(200, "application/json", "{\"status\":\"OK\"}");
}

void setup() {
  Serial.begin(115200);
  delay(100);

  connectWifi(targetSSID, targetPassword);

  server.on("/health", HTTP_GET, handleHealth);
  server.begin();
  Serial.println("HTTP server started.");
}

void loop() {
  if (!isWifiConnected()) {
    Serial.println("\nWiFi disconnected. Reconnecting...");
    connectWifi(targetSSID, targetPassword);

    server.on("/health", HTTP_GET, handleHealth);
    server.begin();
    Serial.println("HTTP server started.");
  }

  server.handleClient();
}
