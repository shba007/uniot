#include "utils-wifi.h"
#include "utils-ntp.h"

const char targetSSID[] = "SKYNET";
const char targetPassword[] = "thehacker@678934";

struct tm* timeinfo;

// Function to ping via Serial every random interval (5-10 sec) showing current GMT and local time.
void pingTime() {
  // Update the global timeinfo with the current time.
  if (!getLocalTime(timeinfo)) {
    Serial.println("Unable to update current time");
    return;
  }
  
  String gmtime = String(formatGMTTime(timeinfo, 0));
  String localtime = String(formatGMTTime(timeinfo, 19800));

  Serial.println("GM Time: " + gmtime + " | Local Time: " + localtime);
}

// Example usage in setup():
void setup() {
  Serial.begin(115200);
  // Seed the random number generator.
  randomSeed(analogRead(0));
  
  connectWifi(targetSSID, targetPassword);
  timeinfo = updateLocalTime();
  
  String gmtime = String(formatGMTTime(timeinfo, 0));
  String localtime = String(formatGMTTime(timeinfo, 19800));

  Serial.println("GM Time: " + gmtime + " | Local Time: " + localtime);
}

void loop() {
  // If WiFi disconnects, try to reconnect.
  if (!isWifiConnected()) {
    Serial.println("\nWiFi disconnected. Reconnecting...");
    connectWifi(targetSSID, targetPassword);
    timeinfo = updateLocalTime();

    String gmtime = String(formatGMTTime(timeinfo, 0));
    String localtime = String(formatGMTTime(timeinfo, 19800));
  
    Serial.println("GM Time: " + gmtime + " | Local Time: " + localtime);
  }

  // Every random 5-10 sec interval, ping with current GMT and local time.
  int interval = random(5000, 10000);
  delay(interval);
  pingTime();
}
