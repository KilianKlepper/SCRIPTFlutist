// rgb.h
// BTF-12V-30L-W, WS2811
// https://github.com/FastLED/FastLED/wiki/Overview
#ifndef RGB_H
#define RGB_H

/// WS2811 LED string setup
#define NUM_LEDS 100
#define DATA_PIN 23

enum rgbEffect
{
    OFF,             // OFF
    BASIC,           // BASIC
    FADE,            // FADE
    RAINBOW,         // RNBW
    RAINBOWGLITTER,  // RGGL
    CONFETTI,        // CNFT
    RUNNINGLIGHT,    // RUNL
    JUGGLE,          // JGL
    SINELON,         // SNLN
    STATIONARY       // STAT
};
void initRGB();

void updateRGBValue(int dec_red, int dec_green, int dec_blue);

void updateRGBeffect(int effect, int brightness, int speed, int cap_hue);

int get_r();
int get_g();
int get_b();

#endif // RGB_H