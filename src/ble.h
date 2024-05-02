#ifndef _IMU_SENSOR_BLE_H_
#define _IMU_SENSOR_BLE_H_

#include <BLEDevice.h>
#include <BLE2902.h>

// #define SERVICE_UUID "93c413cb-f54e-4623-814e-4245134ec45b"
// #define CHARACTERISTIC_UUID "b3e5b202-fb45-4203-8b1b-1ab6cdf6e2cc"

void startService(BLEServer *pServer);

void startAdvertising();


#endif  // _IMU_SENSOR_BLE_H_