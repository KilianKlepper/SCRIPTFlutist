// Interface.cpp
#include <Arduino.h>
#include "interface.h"

bool rising_flag_power = false;
bool falling_flag_power = false;
bool rising_flag_effect = false;
bool falling_flag_effect = false;
bool buttonPressed = false;
bool lastButtonState = false;
bool rgb_power_state = false;
bool effect_state = false;

void initInterface()
{
    // Time critical function call before serial print
    pinMode(MCU_POW, OUTPUT);
    digitalWrite(MCU_POW, HIGH);
    Serial.print("Initializing Interface... ");
    pinMode(CAP_BUT_W, INPUT);
    pinMode(CAP_BUT_Y, INPUT);
    pinMode(CAP_BUT_O, INPUT);
    pinMode(ON_OFF, INPUT);
    pinMode(VIB_MOT, OUTPUT);
    pinMode(RGB_POW, OUTPUT);
    digitalWrite(RGB_POW, HIGH);
    Serial.println("OK");
}

// Button Control
/// true when pressed, on/off control
bool get_Button_OnOff_state()
{
    return !digitalRead(ON_OFF);
}

bool get_RGB_Power_state()
{
    return digitalRead(RGB_POW);
}

/// true when pressed, will increase brrignetss of the rgb
bool get_CapSense_Up_state()
{
    return digitalRead(CAP_BUT_W);
}

/// true when pressed, will lower brrignetss of the rgb
bool get_CapSense_Down_state()
{
    return digitalRead(CAP_BUT_Y);
}

/// true when pressed, will change color effect of the rgb
bool get_CapSense_Effect_state()
{
    return digitalRead(CAP_BUT_O);
}

bool vibration_feedback(bool button_state)
{
    if (button_state)
    {
        digitalWrite(VIB_MOT, HIGH);
    }
    else
    {
        digitalWrite(VIB_MOT, LOW);
    }
    return digitalRead(VIB_MOT);
}

bool toogle_Power_state()
{
    if (get_Button_OnOff_state() || rising_flag_power)
    {
        rising_flag_power = true;
    }
    if (!get_Button_OnOff_state() && rising_flag_power)
    {
        rising_flag_power = false;
        falling_flag_power = true;
        rgb_power_state = !rgb_power_state;
    }
    falling_flag_power = false;
    return rgb_power_state;
}

bool toogle_Effect_state()
{
    if (get_CapSense_Effect_state() || rising_flag_effect)
    {
        rising_flag_effect = true;
    }
    if (!get_CapSense_Effect_state() && rising_flag_effect)
    {
        rising_flag_effect = false;
        falling_flag_effect = true;
        effect_state = !effect_state;
    }
    falling_flag_effect = false;
    return effect_state;
}

bool direction_Alpha_state()
{
    bool currentButtonState = get_CapSense_Down_state();
    if (!lastButtonState && currentButtonState)
    {
        buttonPressed = true;
    }
    if (lastButtonState && !currentButtonState)
    {
        buttonPressed = false;
        up_down = !up_down;
    }

    lastButtonState = currentButtonState;
    return up_down;
}