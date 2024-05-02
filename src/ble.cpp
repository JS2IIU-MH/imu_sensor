#include <BLEDevice.h>
#include <BLE2902.h>

#define SERVICE_UUID "93c413cb-f54e-4623-814e-4245134ec45b"
#define CHARACTERISTIC_UUID "b3e5b202-fb45-4203-8b1b-1ab6cdf6e2cc"

void startService(BLEServer *pServer){
    BLEService *pService = pServer->createService(SERVICE_UUID);

    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    );
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setValue("M5TEST");

    pService->start();
}

void startAdvertising(){
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);

    BLEDevice::startAdvertising();
}

