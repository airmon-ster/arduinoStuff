#include "FastLED.h"

#define DATA_PIN 11

CRGB leds[1];

void setup()
{
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, 1);
    FastLED.setBrightness(30);
}

void loop()
{
   leds[0] = CRGB(150, 0, 0);
   FastLED.show();
   delay(500);
   leds[0] = CRGB(0, 150, 0);
   FastLED.show();
   delay(500);
   leds[0] = CRGB(0, 0, 150);
   FastLED.show();
   delay(500);
}
