/*Function: Motor Controller
 *Author: Marko Jurisic
 *Input Parameters: Bearing, Heading
 *Output Parameters: None
 *
 *Purpose: Initializes Wall-E with the target location from the GCS, and then Wall-E begins driving to the location while reporting it's
 *         poisitional data to the GCS
 */

#ifndef _MOTORCONTROLLER_H    
#define _MOTORCONTROLLER_H    

void motorController(float bearing, float heading, float distance);

#endif
