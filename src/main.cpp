#include <Arduino.h>
#include <Update.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include "pages.h"

const char* ssid     = "MYESP32";
const char* password = "12345678";

WebServer server(80);

void handleRoot_GET() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
}

void handleServerIndex_GET() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
}

void handleUpdate_POST() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
}

void handleUpdate_File_POST() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {  // start with max available size
            Update.printError(Serial);
        }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
        /* flashing firmware to ESP*/
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
            Update.printError(Serial);
        }
    } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {  // true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
            Update.printError(Serial);
        }
    }
}

void setupHotspot() {
    WiFi.softAP(ssid, password);
    Serial.println("Hotspot created:");
    Serial.printf("  SSID: \"%s\"\r\n  Password: \"%s\"\r\n", ssid, password);
    Serial.printf("Connect to the network above, open a browser and navigate to http://%s\r\n", WiFi.softAPIP().toString().c_str());
    Serial.println("To login use \"admin\"/\"admin\"");
}

void setupWebServer() {
    server.on("/", HTTP_GET, handleRoot_GET);
    server.on("/serverIndex", handleServerIndex_GET);
    server.on("/update", HTTP_POST, handleUpdate_POST, handleUpdate_File_POST);
    server.begin();
}

void setup(void) {
    Serial.begin(115200);
    setupHotspot();
    setupWebServer();
}

void loop(void) {
    server.handleClient();
    delay(1);
}
