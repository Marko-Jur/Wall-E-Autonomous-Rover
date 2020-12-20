/* Pin Assignments
 *  
 *  Created by: Marko Jurisic
 *  Date: 17th December 2020
 *  Purpose: Contains all pin references for Wall-E autonomous driving code
 */

#ifndef _PINASSIGNMENTS_H    
 #define _PINASSIGNMENTS_H    

 //BN0055 Assignments:
 
 //Vin 3.3-5V
 //GND Ground
 #define BN0055_SDA 18
 #define BN0055_SCL 19

 
 //NRF24L01 Assignments:
 //Vin 3.3V
 //GND Ground
 //MOSI 11
 //MISO 12
 //SCK 13
 #define CE 4
 #define CSN 5

 //ADAfruit GPS board assignments:

 #define GPS_TX_PIN 11 //GPS module RX goes here
 #define GPS_RX_PIN 12 //GPS module TX goes here



#endif
