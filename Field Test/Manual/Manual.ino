#include "Wall-E_Libraries.h"
#include "Pin_Assignments.h"
#include "Manual_Mode.h"
#include "Landing_Detection.h"

//Values for steering Wall-E, these come from the transmitter and are passed into the manual motor controller function to steer Wall-E
int steering_values[8] = {0,0,0,0,0,0,0,0}; // Right Lateral, Right up/down, Thottle, Left lateral, switch A, switch B, Switch C, Switch D
int LANDED_FLAG = 0;

void setup(){

  rcSetup();
  landing_setup();
}

void loop(){

  rcReader(steering_values);
  LANDED_FLAG = landing_detection(steering_values[4]);
  Serial.println(steering_values[4]);
  if (LANDED_FLAG == 1){
    manualMotorController(steering_values);
  }
}
