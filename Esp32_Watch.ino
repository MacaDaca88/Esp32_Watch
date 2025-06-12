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

const String apiKey = "4a68ce6a2308c37c5359f808959da76b";  // Get this from OpenWeatherMap
const String city = "Christchurch";  // Set your city name
const String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric";  // Request URL
String weatherDescription;
float temperature;
float humidity;
#include "Led.h"
#include "Batt.h"

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

  u8g2.clearBuffer();
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
  // Enable Wi-Fi power saving
  WiFi.setSleep(true);
}

void loop() {
  Time = millis();

  ArduinoOTA.handle();  // OTA handling always active
  static unsigned long lastSync = 0;
  if (WiFi.status() == WL_CONNECTED && millis() - lastSync > 60000) {
    if (timeClient.update()) {
      lastSync = millis();                       // Store for tracking sync frequency
      lastNtpEpoch = timeClient.getEpochTime();  // Store actual time of last successful NTP
    }
  }

  button();
  Menu();
}