#include "Wall-E_Libraries.h"
#include "Pin_Assignments.h"
#include "Nav_System.h"

void setupNav() {
  bno.begin();
  /*
  GPS.begin(9600);  
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // Do not exceed 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);
  mySerial.println(PMTK_Q_RELEASE); */

  current_time = millis();
}

void navSystem(float d_latitude, float d_longitude, float *return_vals) {
  /* return_vals[0] --> c_latitude 
   * return_vals[1] --> c_longitude 
   * return_vals[2] --> distance
   * return_vals[3] --> bearing
   * return_vals[2] --> heading 
   */

  sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

  acc_x = accelerometerData.acceleration.x;
  acc_y = accelerometerData.acceleration.y;
  acc_z = accelerometerData.acceleration.z;

  int8_t boardTemp = bno.getTemp();
  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

   /*
   GPS.read();
   if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
   } 
   
   if((millis() - current_time) > 1000){
    current_time = millis();  
    
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print("| quality: "); Serial.print((int)GPS.fixquality);

    current_latitude = ((int)(GPS.latitude/100.0));
    latitude_min = (current_latitude*(-100)+(float)GPS.latitude);
    current_latitude = ((float)current_latitude + latitude_min/60.0) * (PI/180);

    current_longitude = ((int)(GPS.longitude/100.0));
    longitude_min = (current_longitude*(-100)+(float)GPS.longitude);
    current_longitude = ((float)(current_longitude) + longitude_min/60.0) * (-PI/180);
        
    h_a = sin((destination_latitude - current_latitude)/2) * sin((destination_latitude - current_latitude)/2) + cos(current_latitude) * cos(destination_latitude) * sin((destination_longitude - current_longitude)/2) * sin((destination_longitude - current_longitude)/2);
    h_c = 2 * atan2(sqrt(h_a), sqrt(1-h_a));
    distance_x = h_c * EARTH_RAD;
    bearing = atan2((destination_latitude - current_latitude),(destination_longitude - current_longitude)) * (180/PI);
    
    Serial.print(" | lat:");
    Serial.print(current_latitude * (180/PI), 5);
    Serial.print(" | long:");
    Serial.print(current_longitude * (180/PI), 5);
    Serial.print(" | b:");
    Serial.print(bearing);
    Serial.print(" | h:");
    Serial.print(heading);
    Serial.print(" | dist:");
    Serial.println(distance_x);
   }*/
  
}
