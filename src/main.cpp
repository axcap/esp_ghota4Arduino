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

#include <ESP8266WiFiMulti.h>
#include "LittleFS.h"
#include "ota.h"

#define RELESE_URL "https://api.github.com/repos/axcap/esp_ghota4Arduino/releases/latest"
#define CONNECT_TIMEOUT_MS 10000
auto hostname = "ESP8266 OTA";

ESP8266WiFiMulti wifiMulti;
WiFiClientSecure client;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("Welcome");

  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  File file = LittleFS.open("/wifi_config.txt", "a+");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.println("File Content:");
  while(file.available()){
    auto str = file.readString(); //.split("\n")
    auto ssid = str.substring(0, str.indexOf(";"));
    auto password = str.substring(str.indexOf(";")+1, str.length()-1);
    wifiMulti.addAP(ssid.c_str(), password.c_str());
    Serial.println();
  }
  file.close();

  Serial.println("Initialize WiFi");
  WiFi.hostname(hostname);
  auto status = wifiMulti.run();
  if(status != WL_CONNECTED) {
    Serial.println("WiFi not connected! " + String(status));
    delay(1000);
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  InitOta();
}

void loop()
{
  HandleOTA(RELESE_URL, client);

  delay(60000);
}