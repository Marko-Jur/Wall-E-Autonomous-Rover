/*  RF Communications
 *  
 *  Created by: Mackenzie Mar
 *  Date: 20th February 2021
 *  Purpose: 
 */

#include "Wall-E_Libraries.h"

RF24 radio(7, 8); // CE, CSN
const int NUM_DATA_POINTS = 2;
const int data[NUM_DATA_POINTS] = {};
const char sendData[] = "Message from Wall-E";

const byte address[6] = "00001";

void setupRoverTranceiver()
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
}

void roverListen()
{
  radio.startListening();
  delay(1000);
  if (radio.available())
  {
    radio.read(&data, sizeof(data));
    for (int i = 0; i < NUM_DATA_POINTS; i++)
    {
      //replace following with sending data to wall-e outputs
      Serial.print(data[i]);
      Serial.print("    ");
    }
    Serial.println();
  }
}

void roverSend()
{
  radio.stopListening();
  radio.write(&sendData, sizeof(sendData)); //replace with getting data from Wall-E
  delay(500);
}