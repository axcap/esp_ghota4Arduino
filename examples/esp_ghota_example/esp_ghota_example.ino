#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266httpUpdate.h>
#include <ota.h>

// This string should correspond to github tag used for Releasing (via. Github Actions)
#define VERSION "0.0.1"

// Intended for preserving github API rate limits under development by 
// parsing HTTP Response to get firmware URL
// #define DO_NOT_USE_GITHUB_API

// Replace 'axcap/esp_ghota4Arduino' with your own repos
#ifdef DO_NOT_USE_GITHUB_API
#define RELEASE_URL "https://github.com/axcap/esp_ghota4Arduino/releases/latest"
#else
#define RELEASE_URL "https://api.github.com/repos/axcap/esp_ghota4Arduino/releases/latest"
#endif

#define HOSTNAME "ESP8266 OTA"

WiFiClientSecure client;
WiFiManager wifiManager;

void setup()
{
  Serial.begin(115200);

  Serial.println("Initialize WiFi");
  WiFi.hostname(HOSTNAME);
  WiFi.begin();

  Serial.print("Connecting");
  for (int i=0; WiFi.status() != WL_CONNECTED; i++)
  {
    delay(1000);
    Serial.print(".");

    if(i > 10){
      Serial.println("Fallback AP created");
      wifiManager.autoConnect("ESP Fallback AP");
    }
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  InitOta(VERSION);
}

void loop()
{
  HandleOTA(RELEASE_URL, client);

  delay(60000);
}