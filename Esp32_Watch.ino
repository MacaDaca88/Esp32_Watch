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

int Time = 0;
int oldTime = 0;
#include "Led.h"
#include "Button.h"
#include "menu.h"

const uint8_t pacmanBitmap[] PROGMEM = {
  0b00001111, 0b11100000,
  0b00011111, 0b11110000,
  0b00111111, 0b11111000,
  0b01111110, 0b01111100,
  0b01111100, 0b00111100,
  0b11111000, 0b00011110,
  0b11111000, 0b00001110,
  0b11111000, 0b00000110,
  0b11111000, 0b00001110,
  0b11111000, 0b00011110,
  0b01111100, 0b00111100,
  0b01111110, 0b01111100,
  0b00111111, 0b11111000,
  0b00011111, 0b11110000,
  0b00001111, 0b11100000
};

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

  // Display Pac-Man bitmap
  u8g2.drawXBMP(56, 24, 16, 15, pacmanBitmap);
  u8g2.sendBuffer();
  delay(2000);

  Serial.print("BOOTING.");

  u8g2.setCursor(10, 10);
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 10, " BOOTING. ");
  u8g2.sendBuffer();
  delay(500);

  u8g2.drawStr(0, 20, " Charging Capacitor ");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.drawStr(0, 30, " HacKing Mainframe. ");

  pixels.setPixelColor(0, pixels.Color(255, 0, 0));  // Red
  pixels.show();
  u8g2.sendBuffer();
  delay(500);
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));  // Green
  pixels.show();
  u8g2.print(". ");
  u8g2.sendBuffer();
  delay(500);
  pixels.setPixelColor(0, pixels.Color(0, 0, 255));  // Blue
  pixels.show();
  u8g2.print(". ");
  u8g2.sendBuffer();
  delay(500);
  pixels.setPixelColor(0, pixels.Color(255, 255, 0));  // Gold
  pixels.show();
  u8g2.print(". ");
  u8g2.sendBuffer();
  delay(500);
  pixels.setPixelColor(0, pixels.Color(0, 255, 255));  // Aqua
  pixels.show();
  u8g2.print(". ");
  u8g2.sendBuffer();
  delay(500);
  pixels.setPixelColor(0, pixels.Color(255, 255, 255));  // White
  pixels.show();
  delay(500);
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // Off
  pixels.show();
  u8g2.clearBuffer();
}

void loop() {
  ArduinoOTA.handle();

  Time = millis();

  u8g2.setDrawColor(1);
  button();
  light();

  Batt();
  Menu();

  if (WiFi.status() == WL_CONNECTED) {
    drawWifiSymbol();  // Show Wi-Fi symbol if connected
    timeClient.update();
  } else {
    // If no WiFi connection, draw a line through the symbol
    u8g2.drawLine(115 - 10, 55 - 10, 115 + 10, 55 + 10);  // Diagonal line through symbol
    Menu();
  }
}

void drawWifiSymbol() {
  int x = 115;
  int y = 55;

  u8g2.drawCircle(x, y, 3, U8G2_DRAW_UPPER_RIGHT);
  u8g2.drawCircle(x, y, 6, U8G2_DRAW_UPPER_RIGHT);
  u8g2.drawCircle(x, y, 9, U8G2_DRAW_UPPER_RIGHT);

  u8g2.drawDisc(x, y, 2, U8G2_DRAW_ALL);
  u8g2.setCursor(105, 40);
  u8g2.setFont(u8g2_font_5x7_tr);

  u8g2.print(WiFi.RSSI());  // Display network RSSI
  u8g2.sendBuffer();
}
