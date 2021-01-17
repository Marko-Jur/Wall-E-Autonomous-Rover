#include "Wall-E_Libraries.h"
#include "Pin_Assignments.h"
#include "Nav_System.h"

float d_latitude = 49.264270;
float d_longitude = -123.254017;
float return_vals[5];

void setup(){
  Serial.begin(115200);
  setupNav();
}

void loop(){
  navSystem(d_latitude, d_longitude, return_vals);
  delay(500);
}
