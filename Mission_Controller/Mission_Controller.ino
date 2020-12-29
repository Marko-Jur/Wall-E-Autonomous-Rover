
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
float target_data[2]= {49.264714,-123.252865};
float navigation_data[8] = {1.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00}; //Fix,Satellites, Latitude, Longitude,distance bearing, speed, Heading

void setup(){

    //Initializae Serial port for debugging
    Serial.begin(115200);

    //initializeTarget(target_data);

    //Call setup funciton for the Navigation systen
    setupNav();

    motorSetup();

    setupRfCommunications(TX_MODE); //Pass in 0 to set it up as a transmitter


}

void loop()
{
 
  
  navSystem(target_data[0],target_data[1],navigation_data);

  Serial.print("Fix: ");Serial.print(navigation_data[0]);Serial.print("\t");
  Serial.print("Sat: ");Serial.print(navigation_data[1]);Serial.print("\t");
  Serial.print("Lat: ");Serial.print(navigation_data[2],6);Serial.print("\t");
  Serial.print("Lon: ");Serial.print(navigation_data[3],6);Serial.print("\t");
  Serial.print("Distance: ");Serial.print(navigation_data[4]);Serial.print("\t");
  Serial.print("bearing: ");Serial.print(navigation_data[5]);Serial.print("\t");
  Serial.print("heading: ");Serial.print(navigation_data[7]);Serial.print("\t");
  Serial.print("Speed: ");Serial.print(navigation_data[6],4);Serial.print("\t");
  Serial.println("");
  
  //sendDataRf(navigation_data);
  
  //motorController(navigation_data[5],navigation_data[7],navigation_data[4]);
}
