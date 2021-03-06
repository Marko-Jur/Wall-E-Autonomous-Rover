
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
#include "Manual_Mode.h"
#include "Landing_Detection.h"


//Target location tests
//float target_data[2]= {49.263297,-123.257949}; // Tennis Court
//float target_data[2]= {49.263596,-123.258728}; //Stairs
//float target_data[2]= {49.263572,-123.257980}; 
float target_data[2]= {49.257624,-123.241804}; //Pitch A Warren
//float target_data[2]= {49.257256,-123.241510}; //Pitch B Wright
//float target_data[2]= {49.263572,-123.257980}; //U-hill

//Data from the Navigation system
float navigation_data[7] = {1.00,0.00,0.00,0.00,0.00,3.00,0.00}; //Fix,Satellites, Latitude, Longitude,distance, bearing, Heading
//Channel values from the RC Transmitter
int rc_transmitter_data[8] = {0,0,0,0,0,0,0,0}; // Right Lateral, Right up/down, Thottle, Left lateral, switch A, switch B, Switch C, Switch D

//Landing Flag
int LANDED_FLAG = 0;

void setup(){

    //Initializae Serial port for debugging
    Serial.begin(115200);

    //Call setup funciton for the Navigation systen
    setupNav(); 

    //Setup the motors
    motorSetup();
    
    //Setup the RF communications, note this works but the code in the loop does not work, so it is not there
    setupRfCommunications(TX_MODE); //Pass in 0 to set it up as a transmitter

    //Setup RC Transmitter
    rcSetup();

    //Setting up landing detection
    landing_setup();
    
}

void loop()
{
  //Reads data from the transmitter, 
  rcReader(rc_transmitter_data);

  //Gets the flag from the landing detection function, if high it means the rover is landed
  LANDED_FLAG = landing_detection(rc_transmitter_data[4]);

  //Gets navigation data from BN0055 and GPS, takes in the target location latitude and longitude, stores data into an array.
  navSystem(target_data[0],target_data[1],navigation_data);

  //Comment for data output
  Serial.print("Fix: ");Serial.print(navigation_data[0]);Serial.print("\t");
  Serial.print("Sat: ");Serial.print(navigation_data[1]);Serial.print("\t");
  Serial.print("Lat: ");Serial.print(navigation_data[2],6);Serial.print("\t");
  Serial.print("Lon: ");Serial.print(navigation_data[3],6);Serial.print("\t");
  Serial.print("Switch: ");Serial.print(rc_transmitter_data[4]);Serial.print("\t");
  Serial.print("Distance: ");Serial.print(navigation_data[4]);Serial.print("\t");
  Serial.print("bearing: ");Serial.print(navigation_data[5]);Serial.print("\t");
  Serial.print("heading: ");Serial.print(navigation_data[6]);Serial.print("\t");
  Serial.println("");
  delay(1);
  
  //If the rover has landed, start driving
  if (LANDED_FLAG == 1){
    motorController(navigation_data[5],navigation_data[6],navigation_data[4]);
    Serial.println("Landed");
  }

}

  
  
  
 
