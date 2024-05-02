#include <M5Unified.h>
#include <BLEDevice.h>
#include <BLE2902.h>
#include "ble.h"

bool deviceConnected = false;

void MyServerCallbacks::onConnect(BLEServer* pServer) {
    Serial.println("connect");
    deviceConnected = true;
}

void MyServerCallbacks::onDisconnect(BLEServer* pServer) {
    Serial.println("disconnect");
    deviceConnected = false;
}

void MyChrCallbacks::onRead(BLECharacteristic *pCharacteristic) {
Serial.println("read");
}
void MyChrCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
Serial.println("write");
}



void BLE::startService(BLEServer *pServer){
    BLEService *pService = pServer->createService(SERVICE_UUID);

    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pCharacteristic->setCallbacks(new MyChrCallbacks());
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setValue(BLE_ADV_VALUE);

    pService->start();
}

// BLEAdvertising BLE::*startAdvertising(){
void BLE::startAdvertising(){
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);

    BLEDevice::startAdvertising();

}

void BLE::sendBLEMsg(float yaw, float roll, float pitch, float temp){
    // BLEAdvertisementData sensorAdvetisementData = BLEAdvertisementData();
    // BR_EDR_NOT_SUPPORTED | LE General Discoverable Mode
    // sensorAdvetisementData.setFlags(0x06);

    uint16_t d_yaw = (uint16_t)(yaw * 100);
    uint16_t d_roll = (uint16_t)(yaw * 100);
    uint16_t d_pitch = (uint16_t)(yaw * 100);
    uint16_t d_temp = (uint16_t)(yaw * 100);

    std::string strMsg = "";

    strMsg += (char)(d_yaw & 0xff);             // 下位バイト
    strMsg += (char)((d_yaw >> 8) & 0xff);      // 上位バイト

    strMsg += (char)(d_roll & 0xff);             // 下位バイト
    strMsg += (char)((d_roll >> 8) & 0xff);      // 上位バイト

    strMsg += (char)(d_pitch & 0xff);             // 下位バイト
    strMsg += (char)((d_pitch >> 8) & 0xff);      // 上位バイト

    strMsg += (char)(d_temp & 0xff);             // 下位バイト
    strMsg += (char)((d_temp >> 8) & 0xff);      // 上位バイト

    // sensorAdvetisementData.addData(strMsg);

    // pCharacteristic->setValue(strMsg);
    // pCharacteristic->notify();

}

void BLE::testBLEMsg(float yaw){
    pCharacteristic->setValue(yaw);
    pCharacteristic->notify();
}
