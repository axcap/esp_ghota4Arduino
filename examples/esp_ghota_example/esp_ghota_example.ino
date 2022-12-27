#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ota.h>

#define RELESE_URL "https://github.com/axcap/esp_ghota4Arduino/releases/latest"
// #define RELESE_URL "https://api.github.com/repos/axcap/esp_ghota4Arduino/releases/latest" //use GetUpdatedFirmwareUrlFromApi to parse
#define CONNECT_TIMEOUT_MS 10000
auto hostname = "ESP8266 OTA";

WiFiClientSecure client;
WiFiManager wifiManager;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("Initialize WiFi");
  WiFi.mode(WIFI_STA);
  // wifiManager.resetSettings();
  WiFi.hostname(hostname);
  WiFi.begin();

  Serial.print("Connecting");
  for (int i=0; WiFi.status() != WL_CONNECTED; i++)
  {
    delay(1000);
    Serial.print(".");

    if(i > 5){
      Serial.println("Fallback AP created");
      wifiManager.autoConnect("AP-NAME");
    }
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  InitOta("0.0.1");
}

void loop()
{
  HandleOTA(RELESE_URL, client);

  delay(60000);
}