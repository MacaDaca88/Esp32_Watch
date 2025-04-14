#ifndef OTA_H
#define OTA_H

#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "Tip-jar";
const char* password = "PASSWORD1234LOL";

bool otaInProgress = false;
bool wifiState = false;

void OTAinit() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  /* while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  */
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname("ESP-Watch");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else  // U_SPIFFS
        type = "filesystem";
      otaInProgress = true;  // Set the flag to true

    })
    .onEnd([]() {
      otaInProgress = false;  // Reset the flag
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_ncenB08_tr);
      u8g2.setDrawColor(1);
      u8g2.setCursor(20, 20);
      u8g2.printf("Progress: %u%%\r", (progress / (total / 100)));
      int progressPX = progress / (total / 100);
      u8g2.drawBox(15, 50, progressPX, 64);
      u8g2.setCursor(20, 40);
      u8g2.print("Be Patient Dick");

      u8g2.sendBuffer();
    })
    .onError([](ota_error_t error) {
      u8g2.printf("Error[%u]: ", error);

      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

}
void drawWifiSymbol() {
  int x = 115;     // Center x-coordinate of Wi-Fi symbol
  int y = 59;      // Center y-coordinate of Wi-Fi symbol
  int radius = 9;  // Radius of the largest arc

  if (WiFi.status() == WL_CONNECTED) {
    // Draw Wi-Fi symbol
    u8g2.drawCircle(x, y, 3, U8G2_DRAW_UPPER_RIGHT);
    u8g2.drawCircle(x, y, 6, U8G2_DRAW_UPPER_RIGHT);
    u8g2.drawCircle(x, y, 9, U8G2_DRAW_UPPER_RIGHT);
    // Draw the filled disc at the center
    u8g2.drawDisc(x, y, 2, U8G2_DRAW_ALL);
    // Display network RSSI value near the symbol
    u8g2.setCursor(x - 8, y - 15);
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.print(WiFi.RSSI());
  }
}
void handleWiFiState() {
  if (wifiState) {
    u8g2.setDrawColor(1);
    u8g2.setCursor(30, 40);
    u8g2.print("Turning Wi-Fi ON...");
    WiFi.begin(ssid, password);
  } else {
    u8g2.setDrawColor(1);
    u8g2.setCursor(30, 40);
    u8g2.print("Turning Wi-Fi OFF...");
    WiFi.disconnect();
  }
}

#endif
