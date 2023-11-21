// RGB.cpp
// https://github.com/FastLED/FastLED/wiki/Overview
#include <Arduino.h>
#include "FastLED.h"
#include "rgb.h"


CRGB leds[NUM_LEDS];

void initRGB(){
    Serial.print("Initializing RGB-Strip... ");
    FastLED.addLeds<WS2811, DATA_PIN, BRG>(leds, NUM_LEDS);
    LEDS.showColor(CRGB(0, 0, 0));  // alle LEDs aus
    FastLED.show();
    Serial.println("OK");
}

void updateRGBValue(int dec_red, int dec_green, int dec_blue) {
    Serial.print("Update Color to Value:... ");
    LEDS.showColor(CRGB(dec_red, dec_green, dec_blue));
    Serial.print(dec_red); Serial.print(", ");
    Serial.print(dec_green); Serial.print(", ");
    Serial.println(dec_blue);
    // FastLED.show();
}
