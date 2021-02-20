/*  RF Communications
 *  
 *  Created by: Mackenzie Mar
 *  Date: 20th February 2021
 *  Purpose: 
 */

#ifndef _RF_COMMS_H
#define _RF_COMMS_H

#include "Pin_Assignments.h"

void setupCommandTranceiver();

void commandListen();

void commandSend(int data[NUM_DATA_POINTS]);

#endif