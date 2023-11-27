#include <Arduino.h>
#include <interface.h>
#include <display.h>
#include <ble.h>
#include <rgb.h>

int fluistEffect = OFF;
rgbEffect fluistEffect_test = FADE;
int fluistBrightness = 0;
int fluistSpeed = 10;
int fluistHue = 0;

enum state_machine
{
  FLUIST_ON,
  FLUIST_OFF
};
state_machine state = FLUIST_OFF;

void setup() {
  Serial.begin(115200);
  initInterface();
  initDisplay();
  initRGB();
  initBLE();
  
  Serial.print("Initializing:............ DONE");

}

void loop() {
  // Update Devices#
  bleDisconnect();
  bleConnect();
  // Update OLED


  digitalWrite(RGB_POW, toogle_Power_state());
  switch (state)
  {
  case FLUIST_OFF:
    // Update Display
    displayOnOff_Status(get_RGB_Power_state());

    // Transition Function
    if(toogle_Power_state() || get_valueEFFECT() != OFF) {
      Serial.print("StateMachine set to:......");
      state = FLUIST_ON;
      digitalWrite(RGB_POW, true);
      Serial.println("ON");
    }
    break;
  case FLUIST_ON:
    // Update Display
    displayBLE_Status(get_connectionState());
    displayRGBValue(get_r(), get_g(), get_b());
    displayOnOff_Status(get_RGB_Power_state());
    displayCapSense_Status(get_CapSense_Up_state(), get_CapSense_Down_state(), get_CapSense_Effect_state());
    // Update RGB
    fluistEffect = get_valueEFFECT();
    fluistBrightness = get_valueALPHA();

    updateRGBeffect(fluistEffect, get_valueALPHA(), fluistSpeed, fluistHue);
    
    // Button Update
    if(get_CapSense_Up_state()) {
      write_valueEFFECT(9);
      fluistHue++;
    }

    if(get_CapSense_Down_state()) {
      write_valueALPHA(5);
    }

    // Transition Function
    if(!toogle_Power_state()) {
      Serial.print("StateMachine set to:......");
      state = FLUIST_OFF;
      write_valueEFFECT(OFF);
      digitalWrite(RGB_POW, false);
      displayClear();
      Serial.println("OFF");
    }
    break;
  
  default:
    break;
  }
  
  delay(10);
}
