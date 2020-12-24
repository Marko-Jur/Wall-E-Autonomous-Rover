/*Function: Motor Controller
 *Author: Marko Jurisic
 *Input Parameters: Bearing, Heading
 *Output Parameters: None
 *
 *Purpose: Initializes Wall-E with the target location from the GCS, and then Wall-E begins driving to the location while reporting it's
 *         poisitional data to the GCS
 */

#include "Wall-E_Libraries.h"
#include "Pin_Assignments.h"
#include "Motor_Controller.h"

//constants
#define MAX_SPEED 240 //The max speed Wall-E can go too
#define ACCEPTABLE_RANGE 1.00
#define TERMINATE_DISTANCE 1.00

void motorSetup(){
/*
//Setup the motors
 pinMode(RIGHT_MOTOR_A, OUTPUT);
 pinMode(RIGHT_MOTOR_B, OUTPUT);
 pinMode(RIGHT_MOTOR_ENABLE, OUTPUT);
  
 pinMode(LEFT_MOTOR_A, OUTPUT);
 pinMode(LEFT_MOTOR_B, OUTPUT);
 pinMode(LEFT_MOTOR_ENABLE, OUTPUT);

 
 //Setup Motor Directions:
 digitalWrite(RIGHT_MOTOR_A,HIGH);
 digitalWrite(RIGHT_MOTOR_B,LOW);

 digitalWrite(LEFT_MOTOR_A,HIGH);
 digitalWrite(LEFT_MOTOR_B,LOW);
*/
}

void motorController(float bearing, float heading, float distance){

	//PID constants
	float motor_KP = 1.00;
	float motor_KI = 0.00;
	float motor_KD = 0.00;

	//Setting up a base speed for the motors, eg this is the speed they always turn at and the speeds required to maintain a straight line. These values are determined
 //through experimentation.
	int right_motor_base = 130;
	int left_motor_base = 150;

	//Motor values that we write to the motors
	int right_motor_value = 0;
	int left_motor_value = 0;
	


	//Calculating the direction_error between the heading and the bearing
	float direction_error = bearing - heading;

if (distance > TERMINATE_DISTANCE){
	//If direction_error is greater than the range, that means we must turn right
	if (abs(direction_error) > ACCEPTABLE_RANGE){
		//Calculating the speeds to each motor:
		right_motor_value = right_motor_base - (motor_KP *direction_error);
		left_motor_value = left_motor_base + (motor_KP *direction_error);

		
		//Setting right_motor to 0 to stop the value going below 0
		if (right_motor_value < 0)
			right_motor_value = 0;
		else
			right_motor_value = right_motor_value;
      
    //Setting right_motor to 0 to stop the value going below 0
    if (left_motor_value < 0)
      left_motor_value = 0;
    else
      left_motor_value = left_motor_value;

		//Setting a left motor to max speed to stop the value going over the max
		if (left_motor_value > MAX_SPEED)
			left_motor_value = MAX_SPEED;
		else
			left_motor_value = left_motor_value;
      
    //Setting a left motor to max speed to stop the value going over the max
    if (right_motor_value > MAX_SPEED)
      right_motor_value = MAX_SPEED;
    else
      right_motor_value = right_motor_value;

		//Then writing the values to the motors 
		analogWrite(RIGHT_MOTOR_ENABLE, right_motor_value);
		analogWrite(LEFT_MOTOR_ENABLE, left_motor_value);
	}

 //Just Drive straight
 else{
  
    right_motor_value = right_motor_base;
    left_motor_value = left_motor_base;
    //Then writing the values to the motors 
    analogWrite(RIGHT_MOTOR_ENABLE, right_motor_value);
    analogWrite(LEFT_MOTOR_ENABLE, left_motor_value);
    
 }
 
	  Serial.print("Error = "); Serial.print(direction_error);Serial.print("\t");
    Serial.print("Right motor value = ");Serial.print(right_motor_value);Serial.print("\t");
    Serial.print("Left motor value = "); Serial.print(left_motor_value);Serial.print("\t");
}

else{
    analogWrite(RIGHT_MOTOR_ENABLE, 0);
    analogWrite(LEFT_MOTOR_ENABLE, 0);
  
}

}
