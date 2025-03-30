#ifndef UTILS_NTP_H
#define UTILS_NTP_H

#include <WiFi.h>
#include <time.h>

#define MAX_RETRIES 5

// Define your timezone offset in seconds (e.g., GMT+5:30: 19800 seconds).
const long timezoneOffset = 19800;

struct tm* updateLocalTime();
char* formatGMTTime(struct tm* timeinfo, int timezoneOffset);

#endif  // UTILS_NTP_H