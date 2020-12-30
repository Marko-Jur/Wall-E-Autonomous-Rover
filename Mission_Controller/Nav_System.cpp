#include "Wall-E_Libraries.h"
#include "Pin_Assignments.h"
#include "Nav_System.h"
#include "Rf_communications.h"

//constants:

#define GPSECHO false
#define EARTH_RAD 6371000.0
#define MAGNETIC_DECLINATION 16.1
#define IMU_CALIBRATION_ROUNDS 3000

#define GPSSerial Serial1


Adafruit_GPS GPS(&GPSSerial);
IntervalTimer readIMU;

const float EARTH_CIRCUMFERENCE = 40030170.0;


//variables:

uint16_t BNO055_SAMPLERATE_DELAY_uS = 20000;

float current_time = 0;
float latitude_min, longitude_min, bearing, heading, global_heading, current_latitude, current_longitude, h_a, h_c, distance_x;
float roll, pitch, yaw;
float currentTime, prevTime, elapsedTime;
 
double min_x, max_x, mid_x, acc_x, gyro_x, mag_x, acc_angleX, gyro_anglex, acc_error_x, gyro_error_x, mag_x_proj;
double min_y, max_y, mid_y, acc_y, gyro_y, mag_y, acc_angleY, gyro_angley, acc_error_y, gyro_error_y, mag_y_proj;
double min_z, max_z, mid_z, acc_z, gyro_z, mag_z, gyro_error_z;
 
int imuready, cycle_count;  

sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
 

//function calls:

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);


void setupNav() {
  bno.begin();

  
  //GPS Setup
  
  //while (!Serial);  // uncomment to have the sketch wait until Serial is ready

  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  
  Serial.println("Adafruit GPS library basic test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);

  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);

  readIMU.begin(getHeading, BNO055_SAMPLERATE_DELAY_uS);
}

void navSystem(float d_latitude, float d_longitude, float return_vals[7]) {
  /* return_vals[0] --> fix
   * return_vals[1] --> satellites
   * return_vals[2] --> c_latitude 
   * return_vals[3] --> c_longitude 
   * return_vals[4] --> distance
   * return_vals[5] --> bearing
   * return_vals[6] --> heading 
   */
  

  return_vals[6] = heading;

  //GPS Stuff:////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // read data from the GPS in the 'main loop'
  GPS.read();
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }

  //Actual latitude calculations:
  float temp_latitude = return_vals[2] = (GPS.latitude)/100;
  int integer_latitude = int(temp_latitude);
  float decimal_latitude = ((temp_latitude - integer_latitude)*100)/60;
  float actual_latitude = integer_latitude + decimal_latitude;

  //actual Longitude calculations:
  float temp_longitude = return_vals[3] = (GPS.longitude)/100;
  int integer_longitude = int(temp_longitude);
  float decimal_longitude = ((temp_longitude - integer_longitude)*100)/60;
  float actual_longitude = -(integer_longitude + decimal_longitude);
  
  return_vals[0] = int(GPS.fix);
  return_vals[1] = (GPS.satellites);
  return_vals[2] = actual_latitude;
  return_vals[3] = -actual_longitude;

  //Distance Calculations:
  float distLat = abs(d_latitude - actual_latitude) * 111194.9;
  float distLong = 111194.9 * abs(d_longitude - actual_longitude) * cos(radians((d_latitude + actual_latitude) / 2));
  distance_x = sqrt(pow(distLat, 2) + pow(distLong, 2));

  //Bearing calculations:
  float teta1 = radians(actual_latitude);
  float teta2 = radians(d_latitude);
  float delta1 = radians(d_latitude-actual_latitude);
  float delta2 = radians(d_longitude-actual_longitude);

  //==================Heading Formula Calculation================//

  float y = sin(delta2) * cos(teta2);
  float x = cos(teta1)*sin(teta2) - sin(teta1)*cos(teta2)*cos(delta2);
  float brng = atan2(y,x);
  brng = degrees(brng);// radians to degrees
  bearing = ( ((int)brng + 360) % 360 ); 

  if (bearing > 180){
    bearing -= 360;
  }

  return_vals[4] = distance_x;
  return_vals[5] = bearing;
 
}

void getHeading() {
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
}
