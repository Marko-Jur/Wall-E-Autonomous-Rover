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
  if (mode == RX_MODE){

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
      //radio.setDataRate(RF24_2MBPS);
      //radio.setAutoAck(0);
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

  setupRfCommunications(RX_MODE); //Pass in 0 to set it up as a transmitter
  
  while (target_data[0]==0.00 || target_data[1] == 0.00){
    if (radio.available()) {
      radio.read(&target_data, sizeof(target_data));
    }
  }

  

}

/*Function: Send Data
 *Author: Marko Jurisic 
 *Input Parameters: An array with data drom Wall-E
 *Output Parameters: None
 * 
 * Purpose: Sends Wall-E data via RF to the GUI
 */
void sendDataRf() {
   /*
  send_data[0] = (navigation_data[0]);
  send_data[1] = (navigation_data[1]);
  send_data[2] = (navigation_data[2]);
  send_data[3] = (navigation_data[3]);
  send_data[4] = (navigation_data[4]);
  send_data[5] = (navigation_data[5]);
  send_data[6] = (navigation_data[6]);
  */
  radio.write(&send_data, sizeof(send_data));
  
} 
