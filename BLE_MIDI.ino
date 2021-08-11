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

const byte LATCH = 25;
const byte DATA  = 26;
const byte CLOCK = 14;

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

void setupBle() {
  BLEDevice::init("MIDI HARMONICA");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID));
  pCharacteristic = pService->createCharacteristic(
                      BLEUUID(CHARACTERISTIC_UUID),
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_WRITE_NR
                    );
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->start();
}

void setupKeyboard() {
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, INPUT);
  digitalWrite(LATCH, HIGH);
}

uint64_t readKeyboard() {
  digitalWrite(LATCH, LOW);
  digitalWrite(LATCH, HIGH);
  uint64_t states = shiftIn(DATA, CLOCK, MSBFIRST);
  return states;
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

void processKeyboardChanges(uint64_t keyboard) {
  static uint64_t last_keyboard = 0;
  if (keyboard == last_keyboard) {
    return;
  }
  uint64_t changed = keyboard ^ last_keyboard;
  last_keyboard = keyboard;
  for (int i = 0; i < 64; ++i)
  {
    if (changed & 1) {
      handleChange(i, keyboard & 1);
    }
    changed >>= 1;
    keyboard >>= 1;
  }
}

void handleChange(int key, int state) {
  //  Serial.print("#");
  //  Serial.print(key); // номер кнопки
  //  Serial.print(" -> ");
  //  Serial.println(state); // новое состояние
  uint8_t note = 0;
  uint8_t channel = 0;
  if (key < sizeof(rightKeyboard)) {
    note = rightKeyboard[key];
    channel = CHANNEL0;
  }
  if (state) {
    Serial.print("noteOn -> ");
    Serial.println(note);
    Serial.print("channel -> ");
    Serial.println(channel);
    Serial.println("");
    noteOn(pCharacteristic, note, channel);
  } else {
    Serial.print("noteOff -> ");
    Serial.println(note);
    Serial.print("channel -> ");
    Serial.println(channel);
    Serial.println("");
    noteOff(pCharacteristic, note, channel);
  }
}

void setup() {
  Serial.begin(115200);
  setupKeyboard();
  setupBle();
}

void loop() {
  if (deviceConnected) {
    uint64_t keyboard = readKeyboard();
    processKeyboardChanges(keyboard);
  }
}
