/*  RF Communications
 *  
 *  Created by: Janith Wijekoon
 *  Date: 19th December 2020
 *  Purpose: Hold constants and variables necessary for RF communication.
 */

#ifndef _RF_COMMS_H    
#define _RF_COMMS_H    

//function delarations 

void setupRfCommunications(int mode);
void initializeTarget(float targetData[]);
void sendDataRf(float navigationData[]);



#endif
