#include "FastLED.h"

#define LED_PIN 11
#define LASERTRIP_PIN A1

CRGB leds[1];

void setup()
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, 1);
    FastLED.setBrightness(30);
    pinMode(LASERTRIP_PIN, INPUT);
}

void loop()
{
   int s = digitalRead(LASERTRIP_PIN);
   if (s)
   {
      //Now Motion - GREEN LED
      leds[0] = CRGB(0, 150, 0);
   }
   else
   {
      //Motion Detected - RED LED
      leds[0] = CRGB(150, 0, 0);
   }
   FastLED.show();
   //delay(50);
}
