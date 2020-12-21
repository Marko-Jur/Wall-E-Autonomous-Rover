#include "Wall-E_Libraries.h"
#include "Pin_Assignments.h"
#include "Nav_System.h"

float dummy_1;
float dummy_2;
float dummy_3[5];

void setup(){
  Serial.begin(115200);
  setupNav();
}

void loop(){
  navSystem(dummy_1, dummy_2, dummy_3);
  delay(500);
}
