// RGB.cpp
// https://github.com/FastLED/FastLED/wiki/Overview
#include <Arduino.h>
#include "FastLED.h"
#include "rgb.h"


CRGB leds[NUM_LEDS];

// globals for cool color functions
uint8_t gHue = 0;
uint8_t colour ;
int step = -1;
int center = 0; 
int bright = 50;
int baza = 0;

static void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

static void RGBRainbow() {
    // for(int i = 0; i < NUM_LEDS; i++) {
    //     leds[i] = CHSV(baza+ i * 5, 255, 255);
    // }
    // baza++;
    fill_rainbow( leds, NUM_LEDS, gHue++, 7);
}

static void RGBKonfetti() {
    fadeToBlackBy(leds, NUM_LEDS, 2);
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV(baza++ + random8(64), 200, 255);
    FastLED.setBrightness(bright);
    FastLED.show();
}

static void RGBLauflicht() {
    fadeToBlackBy(leds, NUM_LEDS, 2);
    int pos = beatsin16(13, 0, NUM_LEDS - 1);
    leds[pos] += CHSV(baza++, 255, 192);
}

static void RGBjuggle() {                                               // Several colored dots, weaving in and out of sync with each other
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy( leds, NUM_LEDS, 20);
    byte dothue = 0;
    for ( int i = 0; i < 8; i++) {
      leds[beatsin16(i + 7, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
      dothue += 32;
    }
}

static void RGBsinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

static void RGBGlitterRainbow()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  fill_rainbow( leds, NUM_LEDS, gHue++, 7);
  FastLED.show();
  addGlitter(80);
}


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

void updateRGBeffect(enum rgbEffect effect) {
    switch (effect)
    {
    case BASIC:
        LEDS.showColor(CRGB(10, 10, 10));
        break;
    case RAINBOW:
        RGBRainbow();
        break;
    case RAINBOWGLITTER:
        RGBGlitterRainbow();
        break;
    case KONFETTI:
        RGBKonfetti();
        break;
    case LAUFLICHT:
        RGBLauflicht();
        break;
    case JUGGLE:
        RGBjuggle();
        break;
    case SINELON:
        RGBsinelon();
        break;
    default:
        break;
    }
    FastLED.setBrightness(100);
    FastLED.show();
}

