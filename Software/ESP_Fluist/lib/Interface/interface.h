// ble.h
#ifndef INTERFACE_H
#define INTERFACE_H

/// Pin defines
#define CAP_BUT_W 36
#define CAP_BUT_Y 34
#define CAP_BUT_O 35
#define ON_OFF 39
#define VIB_MOT 26 
#define MCU_POW 13
#define RGB_POW 12


void initInterface();

bool get_CapSense_Up_state();

bool get_CapSense_Down_state();

bool get_CapSense_Effect_state();

bool get_Button_OnOff_state();

bool get_RGB_Power_state();

bool get_MCU_Power_state();

bool vibration_feedback(bool button_state);

#endif // INTERFACE_H
