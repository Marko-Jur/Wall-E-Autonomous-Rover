/*  RF Communications
 *  
 *  Created by: Marko Jurisic
 *  Date: 17th December 2020
 *  Purpose: Contains setup and working code for the RF communication system
 */

#include "Pin_Assignments.h"
#include "Wall-E_Libraries.h"

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
