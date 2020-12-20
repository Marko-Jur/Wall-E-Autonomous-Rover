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

//function calls

RF24 radio(CE, CSN); // CE, CSN

void setupRfCommunications(int mode){

  radio.begin();
  //Setting up RF as a receiver
  if (mode == 1){

    
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();

  }

  //Setting up RF as a transmitter
  else{
 
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
void sendDataRf(float navigationData[]) {

  Serial.println("fejkj");

} 
