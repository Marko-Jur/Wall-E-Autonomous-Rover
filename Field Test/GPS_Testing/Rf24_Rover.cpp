/*  RF Communications
 *  
 *  Created by: Mackenzie Mar
 *  Date: 20th February 2021
 *  Purpose: 
 */

#include "Wall-E_Libraries.h"
#include "Pin_Assignments.h"
#include "Rf24_Rover.h"

RF24 radio(7, 8); // CE, CSN
const int NUM_DATA_POINTS = 2;
const int data[NUM_DATA_POINTS] = {};
//const char sendData[] = "Message from Wall-E"; //debugging
const int sendData[] = {};

const byte address[6] = "00001";
#define const int NUM_DATA_POINTS = 2;

void setupRoverTranceiver()
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
}

int[] roverListen()
{
  radio.startListening();
  delay(1000);
  if (radio.available())
  {
    radio.read(&data, sizeof(data));
    //debugging:
    // for (int i = 0; i < NUM_DATA_POINTS; i++)
    // {
    //   Serial.print(data[i]);
    //   Serial.print("    ");
    // }
    // Serial.println();
  }
  return data;
}

void roverSend()
{
  radio.stopListening();
  //TODO: add with getting data from Wall-E
  radio.write(&sendData, sizeof(sendData));
  delay(500);
}