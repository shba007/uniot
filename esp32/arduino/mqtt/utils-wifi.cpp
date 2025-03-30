#include "utils-wifi.h"

void updateStatusLed(uint8_t state) {
  if (state == WIFI_SCANNING) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  } else if (state == WIFI_CONNECTING) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  } else if (state == WIFI_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else if (state == WIFI_FAILED) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
    delay(500);
  }
}

IPAddress connectWifi(const char* SSID, const char* Password) {
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  int retry = 0;
  bool targetFound = false;

  Serial.println("\nScanning WiFi...");

  while (!targetFound && retry < MAX_RETRIES) {
    updateStatusLed(WIFI_SCANNING);
    int n = WiFi.scanNetworks();
    Serial.println("\nFound " + String(n) + " networks.");

    for (int i = 0; i < n; i++) {
      String ssid = WiFi.SSID(i);
      Serial.println(String(ssid) + " (" + String(WiFi.RSSI(i)) + " dBm)");
      if (ssid == SSID) {
        targetFound = true;
      }
      delay(10);
    }

    if (!targetFound) {
      Serial.println("\nTarget network not found.");
    }

    retry++;
    delay(1000);
  }

  Serial.print("\nConnecting to " + String(SSID));
  retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < MAX_RETRIES) {
    updateStatusLed(WIFI_CONNECTING);
    WiFi.begin(SSID, Password);
    Serial.print(".");
    retry++;
    delay(1000);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to " + String(SSID));
    Serial.println("Local IP: " + WiFi.localIP().toString());
    updateStatusLed(WIFI_CONNECTED);
  } else {
    Serial.println("\nConnection Failed to " + String(SSID));
    Serial.println("Error Code: " + String(WiFi.status()));
    updateStatusLed(WIFI_FAILED);
  }

  return WiFi.localIP();
}

bool isWifiConnected() {
  return WiFi.status() == WL_CONNECTED;
}
