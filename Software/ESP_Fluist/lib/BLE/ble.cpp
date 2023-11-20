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

int red = 0, green = 0, blue = 0;

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


class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic){
    std::string value = pCharacteristic->getValue();

    if(value.length() > 0){

      Serial.print("New Message:............. ");
      
      for(int i=0; i < value.length(); i++){
        Serial.print(value[i]);
      }
      Serial.println();
      // Extract RGB values from the received hex color code
      // Convert std::string to String
      String stringValue = String(value.c_str());
      red = hexToDecimal(stringValue.substring(2, 4));
      green = hexToDecimal(stringValue.substring(4, 6));
      blue = hexToDecimal(stringValue.substring(6, 8));
      // Serial.print("Red: ");
      // Serial.println(red);
      // Serial.print("Green: ");
      // Serial.println(green);
      // Serial.print("Blue: ");
      // Serial.println(blue);
      
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
        oldDeviceConnected = deviceConnected;
        bleDeviceConnection_state = 3;
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