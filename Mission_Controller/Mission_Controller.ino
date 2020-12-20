
/*File: Main Controller REV00
 *Author: Marko Jurisic
 *
 *Purpose: Takes in data from peripherals and outputs a signal to the motors to steer Wall-E
 * 
 */


#include "Pin_Assignments.h"
#include "Wall-E_Libraries.h"
#include "Nav_System.h"
#include "Rf_Communications.h"
#include "Motor_Controller.h"

//Defining Globals
float target_data[2]= {0.00,0.00};
float navigation_data[5] = {0.00,0.00,0.00,0.00,0.00}; // Current Lattidue,Current Longitude, Distance, Bearing, Heading

void setup(){

    //Initializae Serial port for debugging
    Serial.begin(115200);

    //Call setup funciton for the Navigation systen
    setupNav();

    //Setup the RF as a receiver
    setupRfCommunications(1); // Pass in 1 to set it up as a receiver

}

void loop()
{
  initializeTarget(target_data);
  
  navSystem(target_data[0],target_data[1],navigation_data);
  
  sendDataRf(navigation_data);
  
  motorController(navigation_data[2],navigation_data[3],navigation_data[4]);
}
