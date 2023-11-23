// rgb.h
// BTF-12V-30L-W, WS2811
// https://github.com/FastLED/FastLED/wiki/Overview
#ifndef RGB_H
#define RGB_H

/// WS2811 LED string setup
#define NUM_LEDS 10
#define DATA_PIN 23

enum rgbEffect
{
    BASIC,
    RAINBOW,
    RAINBOWGLITTER,
    KONFETTI,
    LAUFLICHT,
    JUGGLE,
    SINELON
};

void initRGB();

void updateRGBValue(int dec_red, int dec_green, int dec_blue);

void updateRGBeffect(int effect);

#endif // RGB_H