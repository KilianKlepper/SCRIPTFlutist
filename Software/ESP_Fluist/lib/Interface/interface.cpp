// Interface.cpp
#include <Arduino.h>
#include "interface.h"

void initInterface(){
    // Time critical function call before serial print
    pinMode(MCU_POW,OUTPUT);
    digitalWrite(MCU_POW, HIGH);
    Serial.print("Initializing Interface... ");
    pinMode(CAP_BUT_W, INPUT);
    pinMode(CAP_BUT_Y, INPUT);
    pinMode(CAP_BUT_O,INPUT);
    pinMode(ON_OFF,INPUT);
    pinMode(VIB_MOT,OUTPUT);
    pinMode(RGB_POW,OUTPUT);
    digitalWrite(RGB_POW, HIGH);
    Serial.println("OK");
}

// Button Control
/// true when pressed, on/off control
bool get_Button_OnOff_state(){ 
    return digitalRead(ON_OFF);
}

/// true when pressed, will increase brrignetss of the rgb
bool get_CapSense_Up_state(){ 
    return digitalRead(CAP_BUT_W);
}

/// true when pressed, will lower brrignetss of the rgb
bool get_CapSense_Down_state(){ 
    return digitalRead(CAP_BUT_Y);
}

/// true when pressed, will change color effect of the rgb
bool get_CapSense_Effect_state(){ 
    return digitalRead(CAP_BUT_O);
}

bool vibration_feedback(bool button_state) {
    if(button_state) {
        digitalWrite(VIB_MOT, HIGH);
    } else {
        digitalWrite(VIB_MOT, LOW);
    }
    return digitalRead(VIB_MOT);
}
