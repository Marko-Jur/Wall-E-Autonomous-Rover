/*  RF Communications
 *  
 *  Created by: Mackenzie Mar
 *  Date: 20th February 2021
 *  Purpose: 
 */

#include "Wall-E_Libraries.h"

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";
char recvData[32] = "";

void setupCommandTranceiver()
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
}

void commandListen()
{
  radio.startListening();
  delay(1000);
  if (radio.available())
  {
    radio.read(&recvData, sizeof(recvData));
    Serial.println(recvData);
  }
}

void commandSend()
{
  radio.stopListening();
  const int latitude = 1;  //replace later
  const int longitude = 2; //replace later
  const int data[2] = {latitude, longitude};
  radio.write(&data, sizeof(data));
  delay(500);
}