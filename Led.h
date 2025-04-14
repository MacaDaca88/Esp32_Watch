#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, LED, NEO_GRB + NEO_KHZ800);

int ledMode = 0;  // LED mode (0 to 3)

bool ledOFF = false;    // Indicates the LED's OFF state
bool ledFlash = false;  // Indicates if the flashing mode is active
bool led = false;       // Indicates the LED's steady state
bool ledFlashColor = false;
bool ledFlashColorRed = false;
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
    case 0:                                            // LED OFF
                                                       /*    led = false;
      ledFlash = false;
      ledFlashColor = false;
      ledFlashColorRed = false;
*/
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // OFF
      pixels.show();
      u8g2.setDrawColor(1);
      u8g2.setFont(u8g2_font_tom_thumb_4x6_mf);
      u8g2.setCursor(100, 10);
      u8g2.print("LED OFF");
      u8g2.sendBuffer();

      break;
    case 1:  // Solid LED
      /*  led = true;
      ledFlash = false;
      ledFlashColor = false;
      ledFlashColorRed = false;
*/
      pixels.setPixelColor(0, pixels.Color(255, 255, 255));  // White
      pixels.show();
      u8g2.setDrawColor(1);
      u8g2.setFont(u8g2_font_tom_thumb_4x6_mf);
      u8g2.setCursor(100, 10);
      u8g2.print("White");
      u8g2.sendBuffer();

      break;
    case 2:  // Flash LED
             /*  ledFlash = true;
      led = false;
      ledFlashColor = false;
      ledFlashColorRed = false;
*/
      // Strobe flashing mode
      if (currentTime - lastFlashTime >= 1000) {  //blink 1x per second
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
      u8g2.setCursor(100, 10);
      u8g2.print("Strobe");
      u8g2.sendBuffer();

      break;
    case 3:  // Flash LED with Color
             /*    ledFlashColor = true;
      led = false;
      ledFlash = false;
      ledFlashColorRed = false;
*/
      // Green flashing mode
      if (currentTime - lastFlashTime >= 300) {  //blink 3x per second
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
      u8g2.setCursor(100, 10);
      u8g2.print("Green");
      u8g2.sendBuffer();

      break;
  }
}
void light() {
  unsigned long currentTime = millis();  // Get the current time
                                         /*
  ledOFF = true;

  if (ledOFF) {
    // LED OFF.
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // OFF
    pixels.show();
  }
  if (led) {
    // LED steady ON.
    pixels.setPixelColor(0, pixels.Color(255, 255, 255));  // White
    pixels.show();
  }

  if (ledFlash) {
    // Strobe flashing mode
    if (currentTime - lastFlashTime >= 1000) {  //blink 1x per second
      // Toggle the LED state
      static bool flashState = false;  // Tracks the ON/OFF state of the flashing
      flashState = !flashState;

      if (flashState) {
        pixels.setPixelColor(0, pixels.Color(255, 255, 255));  // White
      } else {
        //pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // OFF
      }
      pixels.show();

      lastFlashTime = currentTime;  // Update the last flash time
    }
  }

  if (ledFlashColor) {
    // Green flashing mode
    if (currentTime - lastFlashTime >= 300) {  //blink 30x per second
      // Toggle the LED state
      static bool flashState = false;  // Tracks the ON/OFF state of the flashing
      flashState = !flashState;

      if (flashState) {
        pixels.setPixelColor(0, pixels.Color(0, 255, 0));  // Green
      } else {
        //pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // OFF
      }
      pixels.show();

      lastFlashTime = currentTime;  // Update the last flash time
    }
  }
  */
  if (ledFlashColorRed) {
    // Red flashing mode
    if (currentTime - lastFlashTime >= 100) {  //blink 30x per second
      // Toggle the LED state
      static bool flashState = false;  // Tracks the ON/OFF state of the flashing
      flashState = !flashState;

      if (flashState) {
        pixels.setPixelColor(0, pixels.Color(255, 0, 0));  // Red
      } else {
        //  pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // OFF
      }
      pixels.show();

      lastFlashTime = currentTime;  // Update the last flash time
    }
  }
  if (battCharged) {
    pixels.setPixelColor(0, pixels.Color(0, 255, 0));  // Green
    pixels.show();

  } else {
    //   pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // OFF
    //  pixels.show();
  }
  //pixels.show();
}
