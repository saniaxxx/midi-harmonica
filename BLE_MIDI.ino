/*
    BLE_MIDI_HARMONICA
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "notes.h"
#include "keyboards.h"

#define CHANNEL0 0
#define CHANNEL1 1
#define CHANNEL2 2

#define SERVICE_UUID        "03b80e5a-ede8-4b33-a751-6ce34ec4c700"
#define CHARACTERISTIC_UUID "7772e5db-3868-4112-a1a9-f2669d106bf3"

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void setup() {
  Serial.begin(115200);

  BLEDevice::init("ESP32 MIDI");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID));

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      BLEUUID(CHARACTERISTIC_UUID),
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_WRITE_NR
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->start();
}

void loop() {
  if (deviceConnected) {
    // note down
    noteOn(pCharacteristic, C5, CHANNEL0);
    noteOn(pCharacteristic, F5, CHANNEL1);
    delay(500);
    // note up
    noteOff(pCharacteristic, C5, CHANNEL0);
    noteOff(pCharacteristic, F5, CHANNEL1);
    delay(500);
  }
}


void noteOn(BLECharacteristic *midiDevice, uint8_t note, uint8_t channel) {
  uint8_t channels[3] = { 0x90, 0x91, 0x92 };
  uint8_t midiPacket[] = {
    0x80,               // header
    0x80,               // timestamp, not implemented
    channels[channel],  // action
    note,               // pitch
    127                 // velocity
  };
  midiPacket[2] = channels[channel];    // note down, channel 0
  midiPacket[3] = note;                 // note
  midiPacket[4] = 127;                  // velocity
  midiDevice->setValue(midiPacket, 5);  // packet, length in bytes
  midiDevice->notify();
}

void noteOff(BLECharacteristic *midiDevice, uint8_t note, uint8_t channel) {
  uint8_t channels[3] = { 0x80, 0x81, 0x82 };
  uint8_t midiPacket[] = {
    0x80,               // header
    0x80,               // timestamp, not implemented
    channels[channel],  // action
    note,               // pitch
    0                   // velocity
  };
  midiDevice->setValue(midiPacket, 5);  // packet, length in bytes
  midiDevice->notify();
}
