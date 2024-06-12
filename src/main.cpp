#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>
#include <WiFi.h>

#include "pages.h"

const char* ssid     = "MYESP32";
const char* password = "12345678";

AsyncWebServer server(80);

void handleUpdate_GET(AsyncWebServerRequest* request) {
    request->send(200, "text/html", update_GET);
}

void handleUpdate_POST(AsyncWebServerRequest* request) {
    request->send(200, "text/html", update_POST);
    delay(1000);
    ESP.restart();
}

void handleFirmwareUpdate(AsyncWebServerRequest* request, const String& filename, size_t index, uint8_t* data, size_t len, bool final) {
    static size_t total;
    total += len;
    if (!index) {
        Serial.printf("Update: %s", filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {  // start with max available size
            Update.printError(Serial);
        }
    }
    /* flashing firmware to ESP*/
    if (Update.write(data, len) != len) {
        Update.printError(Serial);
    } else {
        Serial.print(".");
    }
    if (final) {
        if (Update.end(true)) {  // true to set the size to the current progress
            Serial.printf("\r\nUpdate Success: %u bytes received\nRebooting...\n", total);
            total = 0;
        } else {
            Update.printError(Serial);
        }
    }
}

void setupHotspot() {
    WiFi.softAP(ssid, password);
    Serial.println("Hotspot created:");
    Serial.printf("  SSID: \"%s\"\r\n  Password: \"%s\"\r\n", ssid, password);
    Serial.printf("Connect to the network above, open a browser and navigate to http://%s/update\r\n", WiFi.softAPIP().toString().c_str());
}

void setupWebServer() {
    server.on("/update", HTTP_GET, handleUpdate_GET);
    server.on("/update", HTTP_POST, handleUpdate_POST, handleFirmwareUpdate);
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    server.begin();
}

void setup() {
    Serial.begin(115200);
    setupHotspot();
    setupWebServer();
}

void loop() {}
