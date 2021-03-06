#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int NUM_DATA_POINTS = 4;
const byte addresses[][6] = {"00001", "00002"};
const int sendData[] = {1, 2, 3, 4};
const int recvData[NUM_DATA_POINTS] = {};
RF24 radio(7, 8); // CE, CSN

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  unsigned long loop_start = millis();
  delay(25);
  radio.stopListening();

  while (millis() - loop_start < 500) {
    radio.write(&sendData, sizeof(sendData));
  }

  delay(25);
  radio.startListening();

  loop_start = millis();
  while (radio.available() && (millis() - loop_start < 500)) {
    radio.read(&recvData, sizeof(recvData));

    for (int i = 0; i < NUM_DATA_POINTS; i++) {
      Serial.print(recvData[i]);
      Serial.print("   ");
    }
    Serial.println();
  }
}