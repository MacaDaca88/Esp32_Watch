#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, LED, NEO_GRB + NEO_KHZ800);

int ledMode = 0;  // LED mode (0 to 4)

bool battFlat = false;
bool battCharged = false;


unsigned long lastFlashTime = 0;          // Tracks the last time the LED toggled in flashing mode
const unsigned long flashInterval = 100;  // Flash interval in milliseconds

void ledStart() {

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

void updateLEDMode() {
  unsigned long currentTime = millis();  // Get the current time

  switch (ledMode) {
    case 0:  // LED OFF

      pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // OFF
      pixels.show();
      u8g2.setDrawColor(1);
      u8g2.setFont(u8g2_font_tom_thumb_4x6_mf);
      u8g2.setCursor(70, 10);
     // u8g2.print("LED OFF");
      break;

    case 1:  // Solid White LED

      pixels.setPixelColor(0, pixels.Color(255, 255, 255));  // White
      pixels.show();
      u8g2.setDrawColor(1);
      u8g2.setFont(u8g2_font_tom_thumb_4x6_mf);
      u8g2.setCursor(70, 10);
     // u8g2.print("White");
      break;

    case 2:  // Strobe White LED

      // Strobe flashing mode
      if (currentTime - lastFlashTime >= 25) {  //blink 50x per second
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
      u8g2.setDrawColor(1);
      u8g2.setFont(u8g2_font_tom_thumb_4x6_mf);
      u8g2.setCursor(70, 10);
      //u8g2.print("White Strobe");
      break;

    case 3:  // Solid Green Led

      pixels.setPixelColor(0, pixels.Color(0, 255, 0));  // Green
      pixels.show();

      u8g2.setDrawColor(1);
      u8g2.setFont(u8g2_font_tom_thumb_4x6_mf);
      u8g2.setCursor(70, 10);
     // u8g2.print("Green");
      break;

    case 4:  // Strobe Green LED

      // Green flashing mode
      if (currentTime - lastFlashTime >= 200) {  //blink 5x per second
        // Toggle the LED state
        static bool flashState = false;  // Tracks the ON/OFF state of the flashing
        flashState = !flashState;

        if (flashState) {
          pixels.setPixelColor(0, pixels.Color(0, 255, 0));  // Green
        } else {
          pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // OFF
        }
        pixels.show();

        lastFlashTime = currentTime;  // Update the last flash time
      }
      u8g2.setDrawColor(1);
      u8g2.setFont(u8g2_font_tom_thumb_4x6_mf);
      u8g2.setCursor(70, 10);
    //  u8g2.print("Green Strobe");
      break;
    case 5:
      // 50 flashing mode
      if (currentTime - lastFlashTime >= 100) {  //blink 5x per second
        // Toggle the LED state
        static bool flashState = false;  // Tracks the ON/OFF state of the flashing
        flashState = !flashState;

        if (flashState) {
          pixels.setPixelColor(0, pixels.Color(255, 0, 0));  // Green
        } else {
          pixels.setPixelColor(0, pixels.Color(0, 0, 255));  // OFF
        }
        pixels.show();

        lastFlashTime = currentTime;  // Update the last flash time
      }
      u8g2.setDrawColor(1);
      u8g2.setFont(u8g2_font_tom_thumb_4x6_mf);
      u8g2.setCursor(70, 10);
     // u8g2.print("50 Strobe");
  }
}

void light() {
  unsigned long currentTime = millis();

  if (battFlat) {
    // Red flashing mode
    if (currentTime - lastFlashTime >= 100) {  //blink 10x per second
      // Toggle the LED state
      static bool flashState = false;  // Tracks the ON/OFF state of the flashing
      flashState = !flashState;

      if (flashState) {
        pixels.setPixelColor(0, pixels.Color(255, 0, 0));  // Red
        pixels.show();
      }

      lastFlashTime = currentTime;  // Update the last flash time
    }
  }
  if (battCharged) {
    pixels.setPixelColor(0, pixels.Color(0, 255, 0));  // Green
    pixels.show();
  }
}