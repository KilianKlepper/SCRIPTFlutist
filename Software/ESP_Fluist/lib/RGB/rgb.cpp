// RGB.cpp
// https://github.com/FastLED/FastLED/wiki/Overview
#include <Arduino.h>
#include "rgb.h"
#include "FastLED.h"

CRGB leds[NUM_LEDS];

void initRGB(){
    Serial.print("Initializing RGB-Strip... ");
    FastLED.addLeds<WS2811, DATA_PIN, BRG>(leds, NUM_LEDS);
    LEDS.showColor(CRGB(0, 0, 0));  // alle LEDs aus
    FastLED.show();
    Serial.println("OK");
}

void updateRGBValue(int red, int green, int blue) {
    Serial.print("Update Color to Value:... ");
    LEDS.showColor(CRGB(red, green, blue));
    Serial.print(red); Serial.print(", ");
    Serial.print(green); Serial.print(", ");
    Serial.print(blue);
    FastLED.show();
}
