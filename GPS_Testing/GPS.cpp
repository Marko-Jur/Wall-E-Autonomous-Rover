#include <Adafruit_GPS.h>
#include "GPS.h"

// what's the name of the hardware serial port?
#define GPSSerial Serial1

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

const float EARTH_RAD =  6371000.0;
const float EARTH_CIRCUMFERENCE = 40030170.0;
float current_time = 0;
float latitude_min, longitude_min, bearing, heading, current_latitude, current_longitude, h_a, h_c, distance_x;

float d_latitude = 49.264714;
float d_longitude = -123.252865;

//Actual distance is 186.32 meters

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

uint32_t timer = millis();


void setupGps()
{
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
}

void gpsData(float gps_data[7]) // run over and over again
{
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }
  
/*
  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    Serial.print("\nTime: ");
    if (GPS.hour < 10) { Serial.print('0'); }
    Serial.print(GPS.hour, DEC); Serial.print(':');
    if (GPS.minute < 10) { Serial.print('0'); }
    Serial.print(GPS.minute, DEC); Serial.print(':');
    if (GPS.seconds < 10) { Serial.print('0'); }
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }

    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 9); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 9); Serial.println(GPS.lon);
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
  }
  */

  //Actual latitude calculations:
  float temp_latitude = gps_data[2] = (GPS.latitude)/100;
  int integer_latitude = int(temp_latitude);
  float decimal_latitude = ((temp_latitude - integer_latitude)*100)/60;
  float actual_latitude = integer_latitude + decimal_latitude;

  //actual Longitude calculations:
  float temp_longitude = gps_data[3] = (GPS.longitude)/100;
  int integer_longitude = int(temp_longitude);
  float decimal_longitude = ((temp_longitude - integer_longitude)*100)/60;
  float actual_longitude = -(integer_longitude + decimal_longitude);

 
  

  gps_data[0] = int(GPS.fix);
  gps_data[1] = (GPS.satellites);
  gps_data[2] = actual_latitude;
  gps_data[3] = -actual_longitude;
  gps_data[6] = (GPS.speed) * 0.514444; //Converting knows to m/s

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

  
  
  /*
  h_a = sin((d_latitude - actual_latitude)/2) * sin((d_latitude - actual_latitude)/2) + cos(actual_latitude) * cos(d_latitude) * sin((d_longitude - actual_longitude)/2) * sin((d_longitude - actual_longitude)/2);
  h_c = 2 * atan2(sqrt(h_a), sqrt(1-h_a));
  distance_x = (h_c * EARTH_RAD);
  */
  //bearing = atan2((d_latitude - actual_latitude),(d_longitude - actual_longitude)) * (180/PI);
  

  gps_data[4] = distance_x;
  gps_data[5] = bearing;

}
