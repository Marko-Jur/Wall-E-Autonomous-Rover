/*  RF Communications
 *  
 *  Created by: Mackenzie Mar
 *  Date: 20th February 2021
 *  Purpose: 
 */

#ifndef _RF_COMMS_H
#define _RF_COMMS_H

#include "Pin_Assignments.h"

void setupRoverTranceiver();

int* roverListen();

void roverSend();

#endif