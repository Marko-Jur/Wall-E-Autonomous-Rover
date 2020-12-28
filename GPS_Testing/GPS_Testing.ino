#include "GPS.h"
#include "Rf_Communications.h"
#include "Wall-E_Libraries.h"
//#include <nRF24L01.h>
//#include <RF24.h>

float gps_data[7] = {1.00,0.00,0.00,0.00,0.00,0.00,0.00}; //Fix,Satellites, Latitude, Longitude,distance bearing, speed

//RF24 radio(9, 10);; // CE, CSN
//const byte address[6] = "00001";

//uint32_t time_a = millis();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  setupGps();
  setupRfCommunications(0);
  /*
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  */
}

void loop() {
  // put your main code here, to run repeatedly:

  gpsData(gps_data);

  
  Serial.print("Fix: ");Serial.print(gps_data[0]);Serial.print("\t");
  Serial.print("Sat: ");Serial.print(gps_data[1]);Serial.print("\t");
  Serial.print("Lat: ");Serial.print(gps_data[2],6);Serial.print("\t");
  Serial.print("Lon: ");Serial.print(gps_data[3],6);Serial.print("\t");
  Serial.print("Distance: ");Serial.print(gps_data[4]);Serial.print("\t");
  Serial.print("bearing: ");Serial.print(gps_data[5]);Serial.print("\t");
  Serial.print("Speed: ");Serial.print(gps_data[6],4);Serial.print("\t");
  Serial.println("");



  //if (millis()-time_a > 10){
    sendDataRf(gps_data);
   // time_a=millis();
  //
  
  
  //radio.write(&gps_data2, sizeof(gps_data2));
  /*
  if (millis()-time_a > 10){
    radio.write(&gps_data, sizeof(gps_data));
    Serial.println("MOTHERFUCKER :p");
    time_a = millis();
  }
  */

}
