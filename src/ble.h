#ifndef _IMU_SENSOR_BLE_H_
#define _IMU_SENSOR_BLE_H_

#include <BLEDevice.h>
#include <BLE2902.h>

#define SERVICE_UUID "93c413cb-f54e-4623-814e-4245134ec45b"
#define CHARACTERISTIC_UUID "b3e5b202-fb45-4203-8b1b-1ab6cdf6e2cc"

#define BLE_ADV_VALUE "M5Stack ADV MSG"

class BLE
{
    public:
        void startService(BLEServer *pServer);

        void startAdvertising();

        void sendBLEMsg(float yaw, float roll, float pitch, float temp);

        void testBLEMsg(float yaw);
    
    private:
        // BLEServer *pServer = NULL;
        // BLEAdvertising *pAdvertising = NULL;
        BLECharacteristic* pCharacteristic = NULL;
};

class MyServerCallbacks: public BLEServerCallbacks {
    public:
        void onConnect(BLEServer* pServer);
        void onDisconnect(BLEServer* pServer);
};
 
class MyChrCallbacks: public BLECharacteristicCallbacks {
    public:
        void onRead(BLECharacteristic *pCharacteristic);
        void onWrite(BLECharacteristic *pCharacteristic);
};



#endif  // _IMU_SENSOR_BLE_H_