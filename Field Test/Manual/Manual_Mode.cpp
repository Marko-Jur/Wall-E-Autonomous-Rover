#include "Wall-E_Libraries.h"
#include "Pin_Assignments.h"
#include "Manual_Mode.h"

//Defining the Receiver object
PulsePositionInput input_values;

//Defining the steering Resolution:
const int STEERING_RESOLUTION = 20;

//Array for values:
int channel_values[8] = {0,0,0,0,0,0,0,0}; // Right Lateral, Right up/down, Thottle, Left lateral, switch A, switch B, Switch C, Switch D
int mapped_channel_values[8] = {0,0,0,0,0,0,0,0}; // Mapped values from 1000-2000 to 0-255

//Values to write to motors
int right_motor_value = 0;
int left_motor_value = 0;

/*Function: RC Setup
 *Author: Marko Jurisic
 *Input Parameters: None
 *Output Parameters: None
 *
 *Purpose: Sets up the receiver
 * 
 */
void rcSetup(){

//Set up the Receiver
 input_values.begin(RCRX);

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


}


/*Function: RC Reader
 *Author: Marko Jurisic
 *Input Parameters: An array of steering values
 *Output Parameters: Modifies the input array with values from the transmitter
 *
 *Purpose: Getting values from the transmitter to steer Wall-E
 * 
 */
void rcReader(int channel_values[]){

int i =0;

channel_values[0] = input_values.read(1);
//Serial.println(channel_values[1]);
channel_values[1] = input_values.read(2);
channel_values[2] = input_values.read(3);
channel_values[3] = input_values.read(4);
channel_values[4] = input_values.read(5);
channel_values[5] = input_values.read(6);
channel_values[6] = input_values.read(7);

//Uncomment this if you would like to see the transmitter values
/*
for (i; i<= 8; i++){
 Serial.println(channel_values[i]);Serial.print("\t");
 delay(1);
}
Serial.print("\n");
*/

}

void manualMotorController(int channel_values[]){

  //Mapping Channel Values
  mapped_channel_values[2] = map(channel_values[2],1000,2003,0,250); //Throttle
  mapped_channel_values[0] = map(channel_values[0],1000,2003,-STEERING_RESOLUTION,STEERING_RESOLUTION); //LRight Lateral

  //Value written to motors:
  right_motor_value = mapped_channel_values[2] + mapped_channel_values[0];
  left_motor_value = mapped_channel_values[2] - mapped_channel_values[0];

  //Allowing Wall-E to turn in place, if throttle is equal to zero we can still write values to the left and right motrs individually
  if (mapped_channel_values[2] == 0 && mapped_channel_values[0] > 3){ //Put greater than 3 just to make sure it's always 0, the transmitter might slightly shift to read 0 as 1 so put 3 to make sure it doesn't turn when the right stick is at 0
    right_motor_value +=150;
    left_motor_value = 0;
    analogWrite(RIGHT_MOTOR_ENABLE,right_motor_value);
  }
  else if (mapped_channel_values[2] == 0 && mapped_channel_values[0] < -3){ //Put greater than 3 just to make sure it's always 0, the transmitter might slightly shift to read 0 as 1 so put 3 to make sure it doesn't turn when the right stick is at 0
    left_motor_value = abs(left_motor_value) + 150;
    right_motor_value = 0;
    analogWrite(RIGHT_MOTOR_ENABLE,left_motor_value);
  }

  //Making sure values are always within motor range:
  else if (right_motor_value > 255){
    right_motor_value = 255; //If the right motor is 255, then we have to minus an extra 20 from the left motor to maintain a constant reoslution of steering
    left_motor_value -= STEERING_RESOLUTION;
  }
  else if (left_motor_value > 255){
    left_motor_value = 255;
    right_motor_value -=STEERING_RESOLUTION;
  }
  else if (right_motor_value < 0){
    right_motor_value = 0;
  }
  else if (left_motor_value < 0){
    left_motor_value = 0;
  }
 
  analogWrite(RIGHT_MOTOR_ENABLE,left_motor_value);
  analogWrite(LEFT_MOTOR_ENABLE,right_motor_value);

//Uncomment to get the values
/*
  Serial.print(right_motor_value);Serial.print("\t");
  Serial.print(left_motor_value);Serial.print("\n");
*/



  
}
