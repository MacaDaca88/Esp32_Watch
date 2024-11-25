#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, LED, NEO_GRB + NEO_KHZ800);

bool ledFlash = false;  // Indicates if the flashing mode is active
bool led = false;       // Indicates the LED's steady state
bool ledFlashColor = false;

unsigned long lastFlashTime = 0;          // Tracks the last time the LED toggled in flashing mode
const unsigned long flashInterval = 100;  // Flash interval in milliseconds

void ledStart(){

  
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
}

void rainbow() {
  pixels.setPixelColor(0, pixels.Color(0, 0, 255));  // Blue
  pixels.show();

  pixels.setPixelColor(0, pixels.Color(255, 0, 0));  // Red
  pixels.show();
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));  // Green
  pixels.show();
}

void light() {
  unsigned long currentTime = millis();  // Get the current time

  if (led) {
    // LED steady ON
    pixels.setPixelColor(0, pixels.Color(255, 255, 255));  // White
    pixels.show();
  } else if (!ledFlash) {
    // LED steady OFF
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // OFF
    pixels.show();
  }
  if (ledFlashColor) {
    rainbow();
  }

  if (ledFlash) {
    // Handle flashing mode
    if (currentTime - lastFlashTime >= flashInterval) {
      // Toggle the LED state
      static bool flashState = false;  // Tracks the ON/OFF state of the flashing
      flashState = !flashState;

      if (flashState) {
        pixels.setPixelColor(0, pixels.Color(255, 255, 255));  // White
      } else {
        pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // OFF
      }
      pixels.show();

      lastFlashTime = currentTime;  // Update the last flash time
    }
  }
}
