#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, LED, NEO_GRB + NEO_KHZ800);


bool ledFlash = false;
bool led = false;

void light() {


  if (led == true) {
    pixels.setPixelColor(0, pixels.Color(255, 255, 255));  // Green
    pixels.show();

  } else {
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // Green
    pixels.show();
  }
  if (ledFlash == true) {
    pixels.setPixelColor(0, pixels.Color(255, 255, 255));  // Green
    pixels.show();

    pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // Green
    pixels.show();
  }
}