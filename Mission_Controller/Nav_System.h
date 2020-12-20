/* Pin Assignments
 *  
 *  Created by: Janith Wijekoon
 *  Date: 18th December 2020
 *  Purpose: Hold constants and variables necessary for navigation and compass subsystems.
 *  
 */

#ifndef _NAV_SYSTEM_H    
 #define _NAV_SYSTEM_H    

 //Compass constants:

 #define GPSECHO true
 #define EARTH_RAD 6371000.0
 #define MAGNETIC_DECLINATION 16.1
 #define IMU_CALIBRATION_ROUNDS 3000


 //compass variables:

 uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;

 float current_time = 0;
 float latitude_min, longitude_min, bearing, heading, current_latitude, destination_latitude, current_longitude, destination_longitude, h_a, h_c, distance_x;
 double min_x, max_x, mid_x, mag_x, acc_x, gyro_x, acc_angleX, gyro_anglex, acc_error_x, gyro_error_x, x_proj;
 double min_y, max_y, mid_y, mag_y, acc_y, gyro_y, acc_angleY, gyro_angley, acc_error_y, gyro_error_y, y_proj;
 double min_z, max_z, mid_z, mag_z, acc_z, gyro_z, gyro_error_z;
 float roll, pitch, yaw;
 float currentTime, prevTime, elapsedTime;
 int imuready, cycle_count;
 
 //function declarations:

 void setup_Nav();
 void nav_System(float d_latitude, float d_longitude, float *return_vals);

 //function calls:

 Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
 SoftwareSerial mySerial(GPS_RX_PIN, GPS_TX_PIN);
 //Adafruit_GPS GPS(&mySerial);

#endif
