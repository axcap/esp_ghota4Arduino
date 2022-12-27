#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoJson.h>
#include "ota.h"
#include "semver_extensions.h"

semver_t current_version;

void InitOta(String version)
{
    current_version = from_string(version.c_str());

    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);

    // Add optional callback notifiers
    ESPhttpUpdate.onStart(update_started);
    ESPhttpUpdate.onEnd(update_finished);
    ESPhttpUpdate.onProgress(update_progress);
    ESPhttpUpdate.onError(update_error);
}

void HandleOTA(String releaseUrl, WiFiClientSecure client)
{
    client.setInsecure();
    auto url = GetUpdatedFirmwareUrl(releaseUrl, client);
    if (url.length() > 0)
    {
        Serial.println("Download URL: " + url);

        ESPhttpUpdate.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
        auto ret = ESPhttpUpdate.update(client, url);

        switch (ret)
        {
        case HTTP_UPDATE_FAILED:
            Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            break;
        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP_UPDATE_NO_UPDATES");
            break;
        case HTTP_UPDATE_OK:
            Serial.println("HTTP_UPDATE_OK");
            break;
        }
    }
}

String GetUpdatedFirmwareUrl(String releaseUrl, WiFiClientSecure client)
{
    String browser_download_url = "";

    HTTPClient https;
    const char *headerKeys[] = { "location" };
    https.collectHeaders(headerKeys, sizeof(headerKeys)/sizeof(headerKeys)[0]);
    https.setFollowRedirects(HTTPC_DISABLE_FOLLOW_REDIRECTS);
    if (https.begin(client, releaseUrl))
    {
        int httpCode = https.GET();
        if (httpCode != HTTP_CODE_FOUND)
        {
            Serial.println("[HTTPS] GET... failed, No redirect");
        }
    
        auto location = https.header("location");
        auto last_index = location.lastIndexOf('/');
        auto semver_str = location.substring(last_index + 1);

        auto _new_version = from_string(semver_str.c_str());
        if(_new_version > current_version){
            auto new_location = String(location + "/firmware.bin");
            browser_download_url = new_location;
            browser_download_url.replace("tag", "download");
        }

        https.end();
    }
    else
    {
        Serial.printf("[HTTPS] Unable to connect\n");
    }

    return browser_download_url;
}

String GetUpdatedFirmwareUrlFromApi(String releaseUrl, WiFiClientSecure client)
{
    auto browser_download_url = "";

    HTTPClient https;
    https.useHTTP10(true);
    https.addHeader("Accept-Encoding", "identity");
    const char *headerKeys[] = { CONTENT_LENGTH_HEADER };
    https.collectHeaders(headerKeys, sizeof(headerKeys)/sizeof(headerKeys)[0]);

    client.setInsecure();
    if (https.begin(client, releaseUrl))
    {
        int httpCode = https.GET();
        if (httpCode < 0)
        {
            Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
            auto length = https.header(CONTENT_LENGTH_HEADER);
            auto content_length = length.toInt();

            DynamicJsonDocument doc(content_length);
            deserializeJson(doc, https.getStream());
            auto _new_version = from_string(doc["name"]);

            if(_new_version > current_version){
                browser_download_url = doc["assets"][0]["browser_download_url"];
            }
        }

        https.end();
    }
    else
    {
        Serial.printf("[HTTPS] Unable to connect\n");
    }

    return browser_download_url;
}

void update_started()
{
    Serial.println("CALLBACK:  HTTP update process started");
}

void update_finished()
{
    Serial.println("CALLBACK:  HTTP update process finished");
}

void update_progress(int cur, int total)
{
    Serial.printf("CALLBACK:  HTTP update process %.1f %%\n", 100.0*cur/total);
}

void update_error(int err)
{
    Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}
