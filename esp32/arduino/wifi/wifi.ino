#include <WiFi.h>
#include "utils-wifi.h"

/* 
WIFI_SCANNING: The LED blinks slowly (500 ms on/off).
WIFI_CONNECTING: The LED blinks quickly (100 ms on/off).
WIFI_CONNECTED: The LED is solid on.
WIFI_FAILED: The LED shows an error pattern (three quick flashes). 
*/

/* const char targetSSID[] = "Neuralink";
const char targetPassword[] = "Neuron@2718"; */

const char targetSSID[] = "SKYNET";
const char targetPassword[] = "thehacker@678934";

void setup() {
  Serial.begin(115200);
  connectWifi(targetSSID, targetPassword);
}

void loop() {
  // If WiFi disconnects, try to reconnect.
  if (!isWifiConnected()) {
    Serial.println("\nWiFi disconnected. Reconnecting...");
    connectWifi(targetSSID, targetPassword);
  }
}
