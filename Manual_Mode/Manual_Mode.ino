#include "Wall-E_Libraries.h"
#include "Pin_Assignments.h"
#include "Manual_Mode.h"

//Values for steering Wall-E, these come from the transmitter and are passed into the manual motor controller function to steer Wall-E
int steering_values[8] = {0,0,0,0,0,0,0,0}; // Right Lateral, Right up/down, Thottle, Left lateral, switch A, switch B, Switch C, Switch D

void setup(){

  rcSetup();
}

void loop(){

  rcReader(steering_values);
  manualMotorController(steering_values);
}
