#ifndef ESP_GHOTA4ARDUINO_H
#define ESP_GHOTA4ARDUINO_H
#include <ESP8266WiFi.h>

#define CONTENT_LENGTH_HEADER "content-length"

void InitOta(String version);
void HandleOTA(String releaseUrl, WiFiClientSecure client);
String GetUpdatedFirmwareUrl(String releaseUrl, WiFiClientSecure client);

void update_started();
void update_finished();
void update_progress(int cur, int total);
void update_error(int err);

#endif
