/*  RF Communications
 *  
 *  Created by: Marko Jurisic
 *  Date: 17th December 2020
 *  Purpose: Contains setup and working code for the RF communication system
 */

#include "Pin_Assignments.h"
#include "Wall-E_Libraries.h"
#include "Rf_Communications.h"

//variables 
const byte address[6] = "00001";

//The array that we send
float send_data[7] = {1.00,0.00,0.00,0.00,0.00,0.00,0.00};



//Setting up Timer to allow the transmitter enough time
uint32_t time_a = millis();

//function calls

RF24 radio(CE, CSN); // CE, CSN

void setupRfCommunications(int mode){

  
  //Setting up RF as a receiver
  if (mode == 1){

    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();

  }

  //Setting up RF as a transmitter
  else{
      radio.begin();
      radio.openWritingPipe(address);
      radio.setPALevel(RF24_PA_MIN);
      radio.stopListening();
  }

}

/*Function: Initialize Target
 *Author: Marko Jurisic 
 *Input Parameters: A pointer to an array which contains the target location
 *Output Parameters: Initialized array
 * 
 * Purpose: Initializes the target array with the target longitude and lattitude
 */
void initializeTarget(float *target_data){
  
  while (target_data[0]==0.00 || target_data[1] == 0.00){
    if (radio.available()) {
      radio.read(&target_data, sizeof(target_data));
    }
  }

  setupRfCommunications(0); //Pass in 0 to set it up as a transmitter

}

/*Function: Send Data
 *Author: Marko Jurisic 
 *Input Parameters: An array with data drom Wall-E
 *Output Parameters: None
 * 
 * Purpose: Sends Wall-E data via RF to the GUI
 */
void sendDataRf(float gps_data[7]) {

  send_data[0] = (gps_data[0]);
  send_data[1] = (gps_data[1]);
  send_data[2] = (gps_data[2]);
  send_data[3] = (gps_data[3]);
  send_data[4] = (gps_data[4]);
  send_data[5] = (gps_data[5]);
  send_data[6] = (gps_data[6]);
  send_data[7] = (gps_data[7]);
  
  if (millis()-time_a > 100){
    radio.write(&send_data, sizeof(send_data));
    time_a = millis();
  }

} 
