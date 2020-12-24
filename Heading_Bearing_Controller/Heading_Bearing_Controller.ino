#include "Wall-E_Libraries.h"
#include "Pin_Assignments.h"
#include "Motor_Controller.h"
#include "Magnetometer_Testing.h"
#include "Manual_Mode.h"


float dummy_longitude = 0.00;
float dummy_latitude = 0.00;
float dummy_distance = 20.00;
float dummy_bearing = -40.00;

int rc_values[8] = {0,0,0,0,0,0,0,0}; // Right Lateral, Right up/down, Thottle, Left lateral, switch A, switch B, Switch C, Switch D

float return_vals[5] = {0.00,0.00,0.00,0.00,0.00}; 
float real_heading = 0; 

/* return_vals[0] --> c_latitude 
 * return_vals[1] --> c_longitude 
 * return_vals[2] --> distance
 * return_vals[3] --> bearing
 * return_vals[4] --> heading 
*/
void setup() {
  Serial.begin(115200);
  motorSetup();
  setupBN0055();
  rcSetup();
  

}

void loop() {

  rcReader(rc_values);
  
  if (rc_values[4] > 1200){
    real_heading = magTester();
    Serial.print("Heading=");Serial.print(real_heading);Serial.print("\t");
    motorController(dummy_bearing, real_heading, dummy_distance);
    Serial.println("");
  }

  else{
    Serial.println(rc_values[4]);
    motorController(dummy_bearing, real_heading, 0);
  }
}
