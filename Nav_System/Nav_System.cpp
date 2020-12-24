#include "Wall-E_Libraries.h"
#include "Pin_Assignments.h"
#include "Nav_System.h"

//constants:

#define GPSECHO true
#define EARTH_RAD 6371000.0
#define MAGNETIC_DECLINATION 16.1
#define IMU_CALIBRATION_ROUNDS 3000


//variables:

uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;

float current_time = 0;
float latitude_min, longitude_min, bearing, heading, current_latitude, current_longitude, h_a, h_c, distance_x;
float roll, pitch, yaw;
float currentTime, prevTime, elapsedTime;
 
double min_x, max_x, mid_x, acc_x, gyro_x, mag_x, acc_angleX, gyro_anglex, acc_error_x, gyro_error_x, mag_x_proj;
double min_y, max_y, mid_y, acc_y, gyro_y, mag_y, acc_angleY, gyro_angley, acc_error_y, gyro_error_y, mag_y_proj;
double min_z, max_z, mid_z, acc_z, gyro_z, mag_z, gyro_error_z;
 
int imuready, cycle_count;  

sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
 

//function calls:

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
Adafruit_GPS GPS(&GPS_SERIAL);

void setupNav() {
  bno.begin();
  
  GPS.begin(9600);  
  GPS_SERIAL.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // Do not exceed 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);
  GPS_SERIAL.println(PMTK_Q_RELEASE); 

  current_time = millis();
}

void navSystem(float d_latitude, float d_longitude, float *return_vals) {
  /* return_vals[0] --> c_latitude 
   * return_vals[1] --> c_longitude 
   * return_vals[2] --> distance
   * return_vals[3] --> bearing
   * return_vals[4] --> heading 
   */
  
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

  acc_x = accelerometerData.acceleration.x;
  acc_y = accelerometerData.acceleration.y;
  acc_z = accelerometerData.acceleration.z;

  gyro_x = angVelocityData.gyro.x;
  gyro_x = angVelocityData.gyro.x;
  gyro_x = angVelocityData.gyro.x;

  mag_x = magnetometerData.magnetic.x;
  mag_y = magnetometerData.magnetic.y;
  mag_z = magnetometerData.magnetic.z;
  
  pitch = orientationData.orientation.y * (PI/180) ;
  roll  = orientationData.orientation.z * (PI/180) ;
  yaw   = orientationData.orientation.x * (PI/180) ;

  mag_x_proj = mag_x * cos(pitch) + mag_y * sin(roll)*sin(pitch) - mag_z*cos(roll)*sin(pitch); //tilt-compensation 
  mag_y_proj = mag_y * cos(roll) + mag_z * sin(roll);

  heading = atan2(mag_y_proj, mag_x_proj) * (180/PI) + MAGNETIC_DECLINATION; //heading with corrected declination
  if (heading > 180)
    heading -= 360;

  return_vals[4] = heading;
  return_vals[3] = 0;
  return_vals[2] = 9001;
  

  /*
  int8_t boardTemp = bno.getTemp();
  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);
  */
  
   
   GPS.read();
   if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))   // also sets newNMEAreceived flag to false
      return;  
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
        
    h_a = sin((d_latitude - current_latitude)/2) * sin((d_latitude - current_latitude)/2) + cos(current_latitude) * cos(d_latitude) * sin((d_longitude - current_longitude)/2) * sin((d_longitude - current_longitude)/2);
    h_c = 2 * atan2(sqrt(h_a), sqrt(1-h_a));
    distance_x = h_c * EARTH_RAD;
    bearing = atan2((d_latitude - current_latitude),(d_longitude - current_longitude)) * (180/PI);

    
    Serial.print(" | lat:");
    Serial.print(current_latitude * (180/PI), 5);
    return_vals[0] = current_latitude * (180/PI);
    Serial.print(" | long:");
    Serial.print(current_longitude * (180/PI), 5);
    return_vals[1] = current_longitude * (180/PI);
    Serial.print(" | b:");
    Serial.print(bearing);
    return_vals[3] = bearing;
    Serial.print(" | h:");
    Serial.print(heading);
    return_vals[4] = heading;
    Serial.print(" | dist:");
    Serial.println(distance_x);
    return_vals[2] = distance_x;
   }
}
