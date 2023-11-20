#include <Arduino.h>
#include <interface.h>
#include <display.h>
#include <ble.h>


void setup() {
  Serial.begin(115200);
  initInterface();
  initDisplay();
  initBLE();

}

void loop() {
  // Update Devices#
  bleDisconnect();
  bleConnect();
  // Update OLED
  displayBLE_Status(get_connectionState());

  displayRGBValue(get_valueRED(), get_valueGREEN(), get_valueBLUE());

  // put your main code here, to run repeatedly:
  displayOnOff_Status(get_Button_OnOff_state());
  displayCapSense_Status(get_CapSense_Up_state(), get_CapSense_Down_state(), get_CapSense_Effect_state());
  delay(500);
}
