// ble.h
#ifndef BLE_H
#define BLE_H

// https://www.uuidgenerator.net/
/// ESP BLE Name
#define DEVICENAME "FLUIST"
/// UUIDs for different services
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

void initBLE();

void bleDisconnect();

void bleConnect();

int get_connectionState();

int get_valueRED();
int get_valueGREEN();
int get_valueBLUE();
int get_valueALPHA();
int get_valueEFFECT();

void write_valueEFFECT(int effect);
void write_valueALPHA(int add, bool up_down);

#endif // BLE_H
