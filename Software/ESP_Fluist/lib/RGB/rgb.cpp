// RGB.cpp
// https://github.com/FastLED/FastLED/wiki/Overview
#include <Arduino.h>
#include "FastLED.h"
#include "rgb.h"
#include "ble.h"


CRGB leds[NUM_LEDS];

// globals for cool color functions
uint8_t gHue = 0;
uint8_t colour ;
int step = -1;
int center = 0; 
int bright = 50;
int baza = 0;
int hue = 0;

static void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}

static void RGBRainbow() {
    fill_rainbow( leds, NUM_LEDS, gHue++, 7);
}

static void RGBFade() {
    fill_solid(leds, NUM_LEDS, CHSV(hue, 255, 255));
    hue++;
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
  addGlitter(80);
  FastLED.show();
  
}

void initRGB(){
    Serial.print("Initializing RGB-Strip... ");
    FastLED.addLeds<WS2811, DATA_PIN, BRG>(leds, NUM_LEDS);
    LEDS.showColor(CRGB(0, 0, 0));  // alle LEDs aus
    FastLED.show();
    Serial.println("OK");
}

int get_r(){
    CRGB currentColor = leds[0];
    return currentColor.r;
}
int get_g(){
    CRGB currentColor = leds[0];
    return currentColor.g;

}
int get_b(){
    CRGB currentColor = leds[0];
    return currentColor.b;
}


void updateRGBValue(int dec_red, int dec_green, int dec_blue) {
    leds[0].r = dec_red;
    leds[0].g = dec_green;
    leds[0].b = dec_blue;
    
    LEDS.showColor(CRGB(dec_red, dec_green, dec_blue));
}

void updateRGBeffect(int effect, int brightness, int speed, int cap_hue) {

    FastLED.setBrightness(brightness);
    switch (effect)
    {
    case OFF:
        // do off stuff
        break;
    case BASIC:
        // LEDS.showColor(CRGB(get_valueRED(), get_valueGREEN(), get_valueBLUE()));
        updateRGBValue(get_valueRED(), get_valueGREEN(), get_valueBLUE());
        FastLED.show();
        break;
    case FADE:
        RGBFade();
        FastLED.show();
        break;
    case RAINBOW:
        RGBRainbow();
        FastLED.show();
        break;
    case RAINBOWGLITTER:
        RGBGlitterRainbow();
        FastLED.show();
        break;
    case CONFETTI:
        RGBKonfetti();
        FastLED.show();
        break;
    case RUNNINGLIGHT:
        RGBLauflicht();
        FastLED.show();
        break;
    case JUGGLE:
        RGBjuggle();
        FastLED.show();
        break;
    case SINELON:
        RGBsinelon();
        FastLED.show();
        break;
    case STATIONARY:
    fill_solid(leds, NUM_LEDS, CHSV(cap_hue, 255, 255));
        FastLED.show();
        break;
    default:
        break;
    }
    if(effect != OFF) {
        delay(speed);
    }
}

