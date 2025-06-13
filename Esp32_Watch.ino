#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "Pins.h"
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
#include "ota.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 43200, 43200000);
unsigned long lastNtpEpoch = 0;

unsigned long Time = 0;
unsigned long oldTime = 0;

const String apiKey = "4a68ce6a2308c37c5359f808959da76b";                                                              // Get this from OpenWeatherMap
const String city = "Christchurch";                                                                                    // Set your city name
const String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric";  // Request URL
String weatherDescription;
float temperature;
float humidity;
int menu = 0;

#include "QR.h"
#include "Led.h"
#include "Batt.h"
#include "SNAKE.h"
#include "Button.h"
#include "menu.h"

void drawPacmanScreen() {
  u8g2.clearBuffer();

  // Draw big Pac-Man (circle with mouth) centered at (64, 32), radius 30
  int cx = 64;
  int cy = 32;
  int r = 30;

  // Draw Pac-Man arc (open mouth from 45° to 315°)
  for (int angle = 45; angle <= 315; angle++) {
    float rad = angle * 3.14159 / 180.0;
    int x = cx + r * cos(rad);
    int y = cy + r * sin(rad);
    u8g2.drawPixel(x, y);
  }

  // Fill Pac-Man (triangle mouth excluded)
  for (int radius = 0; radius < r; radius++) {
    for (int angle = 45; angle <= 315; angle++) {
      float rad = angle * 3.14159 / 180.0;
      int x = cx + radius * cos(rad);
      int y = cy + radius * sin(rad);
      u8g2.drawPixel(x, y);
    }
  }

  // Draw "Maca3D" centered
  u8g2.setFont(u8g2_font_ncenB14_tr);
  const char* text = "Maca3D";
  int textWidth = u8g2.getStrWidth(text);
  u8g2.setCursor((128 - textWidth) / 2, 38);  // Y=38 puts it inside Pac-Man
  u8g2.print(text);

  u8g2.sendBuffer();
}

void setup() {
  OTAinit();
  pixels.begin();

  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);

  pinMode(LED, OUTPUT);

  u8g2.begin();
  u8g2.clearBuffer();

  drawPacmanScreen();
  delay(2000);

  u8g2.clearBuffer();
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 0, 127, 63);
  u8g2.setDrawColor(1);

  u8g2.setCursor(10, 10);
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 10, " BOOTING. ");
  u8g2.sendBuffer();
  delay(500);

  u8g2.drawStr(0, 30, " Charging Capacitor ");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.drawStr(0, 50, " HacKing Mainframe. ");
  ledStart();
  u8g2.clearBuffer();
  timeClient.update();
  // Enable Wi-Fi power saving
  WiFi.setSleep(true);
}
void loop() {
  Time = millis();
  ArduinoOTA.handle();

  static unsigned long lastSync = 0;
  if (WiFi.status() == WL_CONNECTED && millis() - lastSync > 60000) {
    if (timeClient.update()) {
      lastSync = millis();
      lastNtpEpoch = timeClient.getEpochTime();
    }
  }

  // If in game, only handle game logic (snakePlay includes game over screen)
  if (snakePlay) {
    snakeGame();
  } else {
    button();  // Only read buttons in menu or non-game screens
    Menu();    // Main menu system
  }
}
