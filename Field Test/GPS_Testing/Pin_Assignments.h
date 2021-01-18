
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
 #define CE 9
 #define CSN 10

  //Serial port assignments:
 #define GPS_SERIAL Serial1 //Rx --> 0, Tx --> 1

 
 //Motor Pins:
 #define RIGHT_MOTOR_A 2
 #define RIGHT_MOTOR_B 3
 #define RIGHT_MOTOR_ENABLE 14

 #define LEFT_MOTOR_A 4
 #define LEFT_MOTOR_B 5
 #define LEFT_MOTOR_ENABLE 15

//RC Pins
#define RCRX 6



#endif
