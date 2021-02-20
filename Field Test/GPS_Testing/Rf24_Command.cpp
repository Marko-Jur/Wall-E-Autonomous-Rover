/*  RF Communications
 *  
 *  Created by: Mackenzie Mar
 *  Date: 20th February 2021
 *  Purpose: 
 */

#include "Wall-E_Libraries.h"
#include "Pin_Assignments.h"
#include "Rf24_Command.h"

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";
// char recvData[32] = ""; //debugging
int recvData[32] = "";

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
    for (int i = 0; i < NUM_DATA_POINTS; i++)
    {
      //TODO: add labels data variables
      Serial.print(data[i]);
      Serial.print("    ");
    }
    Serial.println();
  }
}

void commandSend(int data[NUM_DATA_POINTS])
{
  radio.stopListening();
  radio.write(&data, sizeof(data));
  delay(500);
}