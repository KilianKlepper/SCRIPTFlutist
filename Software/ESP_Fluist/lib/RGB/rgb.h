// display.h
// BTF-12V-30L-W, WS2811
// https://github.com/FastLED/FastLED/wiki/Overview
#ifndef RGB_H
#define RGB_H

/// WS2811 LED string setup
#define NUM_LEDS 10
#define DATA_PIN 23


void initRGB();

void updateRGBValue(int red, int green, int blue);

#endif // RGB_H