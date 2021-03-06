#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN

const int NUM_DATA_POINTS = 4;
const byte addresses[][6] = {"00001", "00002"};
const int recvData[NUM_DATA_POINTS] = {};
const int sendData[] = {5, 6, 7, 8};

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
  radio.setPALevel(RF24_PA_MIN);
}
void loop() {
  unsigned long loop_start = millis();

  delay(25);
  radio.startListening();
  while (radio.available() && (millis() - loop_start < 500)) {
    radio.read(&recvData, sizeof(recvData));
    for (int i = 0; i < NUM_DATA_POINTS; i++) {
      Serial.print(recvData[i]);
      Serial.print("   ");
    }
    Serial.println();
  }

  delay(25);
  radio.stopListening();

  loop_start = millis();
  while (millis() - loop_start < 500) {
    radio.write(&sendData, sizeof(sendData));
  }
}