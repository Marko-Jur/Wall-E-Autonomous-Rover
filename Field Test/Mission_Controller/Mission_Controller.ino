
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


//Defining Globals
//float target_data[2]= {49.263297,-123.257949}; // Tennis Court
//float target_data[2]= {49.263596,-123.258728}; //Stairs
//float target_data[2]= {49.263572,-123.257980}; 
float target_data[2]= {49.257137,-123.242492}; //Pitch A Warren
//float target_data[2]= {49.257256,-123.241510}; //Pitch B Wright
//float target_data[2]= {49.263572,-123.257980}; //U-hill

float navigation_data[7] = {1.00,0.00,0.00,0.00,0.00,3.00,0.00}; //Fix,Satellites, Latitude, Longitude,distance bearing, speed, Heading
int rc_transmitter_data[8] = {0,0,0,0,0,0,0,0}; // Right Lateral, Right up/down, Thottle, Left lateral, switch A, switch B, Switch C, Switch D

void setup(){

    //Initializae Serial port for debugging
    Serial.begin(115200);

    //initializeTarget(target_data);

    //Call setup funciton for the Navigation systen
    setupNav();

    //Setup the motors
    motorSetup();

    setupRfCommunications(TX_MODE); //Pass in 0 to set it up as a transmitter

    //Setup RC Transmitter
    rcSetup();

    
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
  Serial.print("heading: ");Serial.print(navigation_data[6]);Serial.print("\t");
  Serial.println("");
  delay(1);
  
  motorController(navigation_data[5],navigation_data[7],navigation_data[4]);
  
}

  
  
  
 
