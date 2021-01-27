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
