#include <WiFi.h>
#include <NTPClient.h>

#include <WiFiUdp.h>

#include <Arduino.h>
#include <U8g2lib.h>
#include "Pins.h"
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
#include "ota.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 46800, 14400000);

int button1 = 0;
int button2 = 0;
int button3 = 0;

int button1State = 0;
int button2State = 0;
int button3State = 0;

int Time = 0;
int oldTime = 0;

#include "menu.h"
#include "Led.h"

void setup() {
  Serial.begin(115200);
  OTAinit();
  pixels.begin();

  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);

  pinMode(LED, OUTPUT);

  u8g2.begin();
  u8g2.clearBuffer();

  Serial.print("BOOTING.");

  u8g2.setCursor(10, 10);
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 10, " BOOTING. ");
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));  // Red
  pixels.show();
  u8g2.print(". ");
  u8g2.sendBuffer();
  delay(1000);
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));  // Green
  pixels.show();
  u8g2.print(". ");
  u8g2.sendBuffer();
  delay(1000);
  pixels.setPixelColor(0, pixels.Color(0, 0, 255));  // Blue
  pixels.show();
  u8g2.print(". ");
  u8g2.sendBuffer();
  delay(1000);
  pixels.setPixelColor(0, pixels.Color(255, 255, 0));  // Gold
  pixels.show();
  u8g2.print(". ");
  u8g2.sendBuffer();
  delay(1000);
  pixels.setPixelColor(0, pixels.Color(0, 255, 255));  // Aqua
  pixels.show();
  u8g2.print(". ");
  u8g2.sendBuffer();
  delay(1000);
  pixels.setPixelColor(0, pixels.Color(255, 255, 255));  // White
  pixels.show();
  delay(1000);
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // White
  pixels.show();
}
void loop() {
  ArduinoOTA.handle();

  Time = millis();
  button1 = digitalRead(BUTTON1);
  button2 = digitalRead(BUTTON2);
  button3 = digitalRead(BUTTON3);

  Menu();
  drawWifiSymbol();  // Show Wi-Fi symbol if connected
  //light();
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.update();
  } else {
    Menu();
    //scanWifiNetworks();
  }
}

void drawWifiSymbol() {
  // Coordinates for the WiFi symbol (adjust as needed)
  int x = 115;
  int y = 55;

  // Draw WiFi arcs
  u8g2.drawCircle(x, y, 3, U8G2_DRAW_UPPER_RIGHT);
  u8g2.drawCircle(x, y, 6, U8G2_DRAW_UPPER_RIGHT);
  u8g2.drawCircle(x, y, 9, U8G2_DRAW_UPPER_RIGHT);

  // Draw small dot (WiFi source)
  u8g2.drawDisc(x, y, 2, U8G2_DRAW_ALL);

  // If no WiFi connection, draw a line through the symbol
  if (!WL_CONNECTED) {
    u8g2.drawLine(x - 10, y - 10, x + 10, y + 10);  // Diagonal line through symbol
  }
  u8g2.sendBuffer();
}
// Function to scan for available Wi-Fi networks
void scanWifiNetworks() {

  int n = WiFi.scanNetworks();  // Perform the network scan

  u8g2.clearBuffer();
  u8g2.setCursor(10, 10);
  u8g2.setFont(u8g2_font_ncenB08_tr);

  if (n == 0) {
    u8g2.print("No Networks Found");
  } else {
    u8g2.print("Wi-Fi Networks:");
    for (int i = 0; i < n && i < 4; i++) {  // Limit to top 3 networks
      u8g2.setCursor(10, 20 + (i * 10));
      u8g2.print(WiFi.SSID(i));  // Display network SSID
      u8g2.print(WiFi.RSSI(i));  // Display network RSSI
      u8g2.sendBuffer();
    }
  }
}