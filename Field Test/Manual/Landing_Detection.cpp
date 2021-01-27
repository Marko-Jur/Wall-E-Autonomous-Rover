/*Function: Landing Detection
 *Author: Marko Jurisic
 *Input Parameters: None
 *Output Parameters: Landing Flag
 *
 *Purpose: Checks if Wall-E has landed and outputs a flag if it has
 */



#include "Wall-E_Libraries.h"
#include "Pin_Assignments.h"
#include "Landing_Detection.h"

const int SERVO_DETACH = 110;
const int SERVO_ENGAGED = 20;

PWMServo landingServo;  // create servo object to control a servo
int LANDED = 0;

void landing_setup(){

 //landingServo.attach(SERVO);  
 landingServo.attach(SERVO, 1000, 2000); // some motors need min/max setting

}

int landing_detection(int landing_switch){


if (landing_switch > 1800){
  landingServo.write(SERVO_DETACH);
  LANDED = 1;
  return 1;
}
else{
  landingServo.write(SERVO_ENGAGED);
  if (LANDED == 0){
    return 0;
  }

  else{
    return 1;
  }
}

}
