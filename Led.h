#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, LED, NEO_GRB + NEO_KHZ800);

void light() {

  pixels.setPixelColor(0, pixels.Color(255, 255, 255));  // Green
  pixels.show();
}