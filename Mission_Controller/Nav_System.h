/* Pin Assignments
 *  
 *  Created by: Janith Wijekoon
 *  Date: 18th December 2020
 *  Purpose: Hold constants and variables necessary for navigation and compass subsystems.
 */

#ifndef _NAV_SYSTEM_H    
 #define _NAV_SYSTEM_H 
 
 //function declarations:

 void setupNav();
 void navSystem(float d_latitude, float d_longitude, float return_vals[8]);

 

#endif
