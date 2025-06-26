#ifndef OTA_H
#define OTA_H

#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "Tip-jar";
const char* password = "PASSWORD1234LOL";

const char* ssid2 = "NotYours";
const char* password2 = "PayMe4It";

bool otaInProgress = false;
bool wifiState = false;

void OTAinit() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for a few seconds to connect
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 5000) {
    delay(100);
  }

  if (WiFi.status() == WL_CONNECTED) {
  } else {
    WiFi.disconnect();
    WiFi.begin(ssid2, password2);

    // Wait again for backup connection
    startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 5000) {
      delay(100);
    }
  }
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
    setCpuFrequencyMhz(240);

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

      // Fonts
      u8g2.setFont(u8g2_font_6x13_tf);  // clean and readable
      u8g2.setDrawColor(1);

      // Title Text
      u8g2.setCursor(20, 14);
      u8g2.print("Firmware Update");

      // Progress percentage
      int percent = (progress * 100) / total;
      char buf[16];
      sprintf(buf, "Progress: %3d%%", percent);
      u8g2.setCursor(25, 30);
      u8g2.print(buf);

      // Progress bar background
      int barX = 10;
      int barY = 35;
      int barW = 110;
      int barH = 15;
      u8g2.drawFrame(barX, barY, barW, barH);

      // Progress bar fill
      int progressPX = (percent * (barW - 2)) / 100;
      u8g2.drawBox(barX + 1, barY + 1, progressPX, barH - 2);

      // Footer message
      u8g2.setCursor(20, 60);
      u8g2.print("Please wait...");

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
void drawWifiSymbol(U8G2& u8g2) {
  int x = 115;             // X position of the center
  int y = 60;              // Y position of the base
  int rssi = WiFi.RSSI();  // Get signal strength
  int level = 0;

  // Determine signal level
  if (rssi > -50) level = 3;       // Excellent
  else if (rssi > -60) level = 2;  // Good
  else if (rssi > -70) level = 1;  // Fair
  else level = 0;                  // Weak

  // Always draw center dot
  u8g2.drawDisc(x, y, 1, U8G2_DRAW_ALL);

  // Draw arcs based on signal level
  if (level >= 1)
    u8g2.drawCircle(x, y, 4, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT);
  if (level >= 2)
    u8g2.drawCircle(x, y, 7, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT);
  if (level >= 3)
    u8g2.drawCircle(x, y, 10, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT);
}

void handleWiFiState() {
  if (wifiState) {
    u8g2.setDrawColor(1);
    u8g2.setCursor(30, 40);
    u8g2.print("Turning Wi-Fi ON...");
    WiFi.begin(ssid, password);
    // Wait for a few seconds to connect
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 3000) {
    }

    if (WiFi.status() == WL_CONNECTED) {
    } else {
      WiFi.disconnect();
      WiFi.begin(ssid2, password2);

      // Wait again for backup connection
      startAttemptTime = millis();
      while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 2000) {
      }
    }
  } else {
    u8g2.setDrawColor(1);
    u8g2.setCursor(30, 40);
    u8g2.print("Turning Wi-Fi OFF...");
    WiFi.disconnect();
  }
}

#endif