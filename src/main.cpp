/*
  This example downloads sketch update over WiFi network.
  You can choose between HTTP and HTTPS connection.
  In HTTPS case, remember to flash the server root CA certificate
  using WiFi101/WiFiNINA updater tool.
  It doesn't start the OTA upload sever of the ArduinoOTA library,
  it only uses the InternalStorage object of the library
  to store and apply the downloaded binary file.

  To create the bin file for update of a SAMD board (except of M0),
  use in Arduino IDE command "Export compiled binary".
  To create a bin file for AVR boards see the instructions in README.MD.
  To try this example, you should have a web server where you put
  the binary update.
  Modify the constants below to match your configuration.

  Created for ArduinoOTA library in December 2020
  by Nicola Elia
  based on Juraj Andrassy sample sketch 'OTASketchDownload'
*/

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include "ota.h"

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

  InitOta("0.0.4-rc.1");
}

void loop()
{
  HandleOTA(RELESE_URL, client);

  delay(60000);
}