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
#include "Motor_Controller.h"

const int SERVO_DETACH = 110;
const int SERVO_ENGAGED = 20;

int LANDING_SEQUENCE_EXECUTED_FLAG = 0;

PWMServo landingServo;  // create servo object to control a servo

void landing_setup(){

 //landingServo.attach(SERVO);  
 landingServo.attach(SERVO, 1000, 2000); // some motors need min/max setting

}

int landing_detection(int landing_switch){


if (landing_switch > 1800){
  
  if (LANDING_SEQUENCE_EXECUTED_FLAG == 0){
    landingServo.write(SERVO_DETACH);
    delay(1000);
    motorController(80,80,100);
    delay(1000);
    landingServo.write(SERVO_ENGAGED);
    LANDING_SEQUENCE_EXECUTED_FLAG = 1;
  }
  
  return 1;
}
else{
  landingServo.write(SERVO_ENGAGED);
  return 0;
}

}
