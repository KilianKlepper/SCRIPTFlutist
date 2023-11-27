#include <Arduino.h>
#include <interface.h>
#include <display.h>
#include <ble.h>
#include <rgb.h>

rgbEffect appEffect = RAINBOWGLITTER;

void setup() {
  Serial.begin(115200);
  initInterface();
  initDisplay();
  initBLE();
  initRGB();
  Serial.print("Initializing RGB-Strip... ");
  Serial.println("Initializing Done....Ready");


}

void loop() {
  // Update Devices#
  bleDisconnect();
  bleConnect();
  // Update OLED
  displayBLE_Status(get_connectionState());

  displayRGBValue(200, 200, 200);
  updateRGBValue(get_valueRED(), get_valueGREEN(), get_valueBLUE());
  updateRGBeffect(get_valueEFFECT());
  // updateRGBeffect(appEffect);
  // updateRGBValue(255, 255, 255);
  // put your main code here, to run repeatedly:
  displayOnOff_Status(get_Button_OnOff_state());
  displayCapSense_Status(get_CapSense_Up_state(), get_CapSense_Down_state(), get_CapSense_Effect_state());
  delay(20);
}
