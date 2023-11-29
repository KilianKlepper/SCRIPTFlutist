// custom_functions.cpp
#include "ble.h"
#include <Arduino.h>
#include <string>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

bool bleService_state = false;
bool bleAdvertising_state = false;
int bleDeviceConnection_state = 0;

uint8_t red = 0, green = 0, blue = 0;
int effect = 0;
uint8_t alpha = 200;
bool up_down = false;

int hexToDecimal(String hexString) {
  long decimalValue = strtol(hexString.c_str(), NULL, 16);
  return (int)decimalValue;
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};


class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();

    Serial.print("New Message (Hex): ");
    
    for (int i = 0; i < value.length(); i++) {
      Serial.print(String(value[i], HEX));
      Serial.print("");
    }
    Serial.println();

    if (value.length() == 5 * sizeof(int8_t)) {
      int receivedValues[5];
      for (int i = 0; i < 5; ++i) {
        receivedValues[i] = value[i];
      }
      // Now 'receivedValues' contains the 4 integers sent from Flutter
      effect = receivedValues[0];
      alpha = receivedValues[1];
      red = receivedValues[2];
      green = receivedValues[3];
      blue = receivedValues[4];

      Serial.println("eARGB: " + String(effect)+' ' + String(alpha)+' ' + String(red)+' ' + String(green)+' ' + String(blue));

      // Continue with your processing...
    } else {
      Serial.println("Received value is too short.");
    }
  }
};

void initBLE() {
    
    Serial.print("Initializing BLE......... ");
    // Create the BLE Device
    bleDeviceConnection_state = 1;
    BLEDevice::init(DEVICENAME);
    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                      );


    pCharacteristic->setCallbacks(new MyCallbacks());
    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
    // Create a BLE Descriptor
    pCharacteristic->addDescriptor(new BLE2902());

    // Start the service
    pService->start();
    Serial.println("OK");
    bleService_state = true;

    // Start advertising
    Serial.print("Start Advertising........ ");
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("OK");
    bleAdvertising_state = true;
    bleDeviceConnection_state = 2;
    Serial.println("Waiting for Client.......");
}

void bleDisconnect() {
    if (!deviceConnected && oldDeviceConnected) {
        Serial.print("Start Advertising........ ");
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("OK");
        oldDeviceConnected = deviceConnected;
        bleDeviceConnection_state = 2;
    }
}

void bleConnect() {
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        Serial.print("Device connected......... ");
        oldDeviceConnected = deviceConnected;
        bleDeviceConnection_state = 3;
        Serial.println("OK");
    } 
}

/// @brief 
/// @return 
int get_connectionState() {
    return bleDeviceConnection_state;
}

int get_valueRED() {
    return red;
}
int get_valueGREEN() {
    return green;
};
int get_valueBLUE() {
    return blue;
}

int get_valueALPHA() {
    return alpha;
}
int get_valueEFFECT() {
    return effect;
}

void write_valueEFFECT(int set_effect){
  effect = set_effect;
}
void write_valueALPHA(int add){
  uint8_t current_alpha = get_valueALPHA();
  if (current_alpha > (255-add) || current_alpha < add) {
    up_down = !up_down;
  }
  if(up_down) alpha = current_alpha + add;
  if(!up_down) alpha = current_alpha - add;
}