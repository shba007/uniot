#include "utils-ntp.h"

// Updates system time via NTP and returns a pointer to the updated timeinfo.
// Returns NULL if synchronization fails.
struct tm* updateLocalTime() {
  // Global variable to hold the NTP-synchronized GMT time.
  static struct tm timeinfo;

  // Configure NTP servers. Using 0 offset ensures we get GMT.
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("\nWaiting for NTP time synchronization");
  int retry = 0;
  while (!getLocalTime(&timeinfo) && retry < MAX_RETRIES) {
    delay(500);
    Serial.print(".");
    retry++;
  }
  Serial.println();

  if (retry == MAX_RETRIES) {
    Serial.println("Failed to obtain time");
    return NULL;
  } else {
    return &timeinfo;
  }
}

// Converts the given GMT time (pointed to by 'timeinfo') to local time by applying
// the provided timezone offset (in seconds). Returns a pointer to a static buffer
// containing the formatted local time string.
char* formatGMTTime(struct tm* timeinfo, int timezoneOffset) {
  if (timeinfo == NULL) {
    Serial.println("Unable to get GMT time");
    return "";
  }

  // Convert the GMT time to time_t, add the timezone offset.
  time_t localTime_t = mktime(timeinfo) + timezoneOffset;

  // Convert back to struct tm for formatting.
  struct tm localTimeInfo;
  localtime_r(&localTime_t, &localTimeInfo);

  // Use a static buffer so the result remains valid after function returns.
  static char localTimeStr[64];
  strftime(localTimeStr, sizeof(localTimeStr), "%Y-%m-%d %H:%M:%S", &localTimeInfo);

  return localTimeStr;
}