#ifndef UTILS_WIFI_H
#define UTILS_WIFI_H

#include <WiFi.h>

// Define LED pin
#define LED_BUILTIN 4

// Define WiFi states
#define WIFI_SCANNING 0
#define WIFI_CONNECTING 1
#define WIFI_CONNECTED 2
#define WIFI_FAILED 3

// Max retries for connection attempts
#define MAX_RETRIES 5

void updateStatusLed(uint8_t state);
IPAddress connectWifi(const char* SSID, const char* Password);
bool isWifiConnected(); // Function to check WiFi connection

#endif // UTILS_WIFI_H
