#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const byte addresses[][6] = {"00001", "00002"};
const int NUM_DATA_POINTS = 32;
const char sendData[NUM_DATA_POINTS] = "Message from command............";
const char recvData[NUM_DATA_POINTS] = {};
RF24 radio(7, 8); // CE, CSN

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  delay(500);

  radio.write(&sendData, sizeof(sendData));

  delay(25);
  radio.startListening();

  while (radio.available()) {
    radio.read(&recvData, sizeof(recvData));
    Serial.println(recvData);
  }


  delay(25);
  radio.stopListening();
}