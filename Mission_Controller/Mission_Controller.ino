/*File: Main Controller REV00
 *Author: Marko Jurisic
 *
 *Purpose: Takes in data from peripherals and outputs a signal to the motors to steer Wall-E
 * 
 */


#include "Pin_Assignments.h"
#include "Libraries.h"
#include "Nav_System.h"

void setup(){

    //Defining Globals
    const float TERMINATE_DISTANCE = 1.00;

    //Initializae Serial port for debugging
    Serial.begin(115200);

    //Call setup funciton for the Navigation systen
    setupNav();

    //Call setup function for receiver (manual control)
    //setupReceiver();
}

void loop()
{
    if (MODE_SWITCH == 1) //Check Switch 1 to check which mode Wall-E is in
        Serial.println("HIGH");
        autonomousMode();
    else 
        manualMode();
        Serial.println("LOW");
}

/*Function: Autonomous Mode
 *Author: Marko Jurisic
 *Input Parameters: None
 *Output Parameters: None
 *
 *Purpose: Initializes Wall-E with the target location from the GCS, and then Wall-E begins driving to the location while reporting it's
 *         poisitional data to the GCS
 */
void autonomousMode(){

    //Variable definitions:
    float target_data[3] = {0.00,0.00,0.00}; //Target Lattitude and Longitude, mission status
    float navigation_data = {}; // Current Lattidue,Current Longitude, Distance, Bearing, Heading
    
    //Setup NRF24L01 as a receiver
    //Call setup function for RF communications
    setupRfCommunications(1); // Pass in 1 to set it up as a receiver

    while (target_data[0]==0.00 || target_data[1] == 0.00){
        if (radio.available()) {
        radio.read(&target_data, sizeof(target_data));
         }
    }

    
    setupRfCommunications(0); //Pass in 0 to set it up as a transmitter

    while (MISSION_COMPLETE == FALSE){
      
        navigation_data = Navsystem(target_data[0],target_data[1]);
        if (navigation_data[2] == TERMINATE_DISTANCE){
          break();
        }
    }

    //Set mission status to 1 for complete
    target_data[2] = 1.00;
    radio.write(&target_data, sizeof(target_data));
}

/*Function: Manual Mode
 *Author: Marko Jurisic
 *
 *Input parameters: None
 *Output parameters: None
 *
 *Purpose: Initializes Wall-E with the target location from the GCS, and then Wall-E begins driving to the location while reporting it's
 *         poisitional data to the GCS
 */
 
void manualMode(){

Serial.println("jdwiwp");
  
}
  
