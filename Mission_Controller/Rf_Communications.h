/*  RF Communications
 *  
 *  Created by: Janith Wijekoon
 *  Date: 19th December 2020
 *  Purpose: Hold constants and variables necessary for RF communication.
 */

#ifndef _RF_COMMS_H    
#define _RF_COMMS_H   


//constants

#define TX_MODE 44
#define RX_MODE 69

//function delarations 

void setupRfCommunications(int mode);
void initializeTarget(float targetData[]);
void sendDataRf(float navigation_data[8]);



#endif
