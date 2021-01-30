/*Function: Landing Detection
 *Author: Marko Jurisic
 *Input Parameters: None
 *Output Parameters: Landing Flag
 *
 *Purpose: Checks if Wall-E has landed and outputs a flag if it has
 */



#include "Wall-E_Libraries.h"
#include "Pin_Assignments.h"
#include "Landing_Detection.h"
#include "FIR_Filter.h"
#include "stdint.h"

const int SERVO_DETACH = 2100;
const int SERVO_ENGAGED = 1200;
const float ERROR_THRESHOLD = 0.01;

Servo LANDING_SERVO;

//static variables
static  sensors_event_t accel_data;
static double accx = 0;
static double accy = 0;
static double accz = 0;
static double old_accx = 0;
static double old_accy = 0;
static double old_accz = 0;

static  sensors_event_t mag_data;
static double magx = 0;
static double magy = 0;
static double magz = 0;

//filter-related info
static FIRFilter accx_filter;
static FIRFilter accy_filter;
static FIRFilter accz_filter;
const static uint8_t filter_order = 15;
static float filter_coeff[15] = {-0.005071,0.011302,0.016822,-0.029808,-0.065989,0.051410,
0.302127, 0.438786,0.302127,0.051410, -0.065989,-0.029808,0.016822,0.011303,-0.005071};
static float filter_buff_accx[15] = {0};
static float filter_buff_accy[15] = {0};
static float filter_buff_accz[15] = {0};

//bno instance
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28); 

//static functions
static void landing_update_acceleration(void);
static void landing_update_magnetics(void);
static bool landing_acc_reading_is_stable(void);


//global functions
void landing_setup(void);
int landing_detection(int landing_switch);

void landing_setup(){
  
      Serial.begin(115200);
    
      if (!bno.begin())
      {
        for(;;){Serial.println("No BNO055 detected");} //shoots the error if disconnected
      }
      bno.setExtCrystalUse(true);//use on-board crystal
      uint8_t dummy_sys = 0, dummy_gyro = 0, accel = 0, magn = 0; //other calibration flags are dummy as we are only using
                                                         //accelerometer and compass for landing

      bno.getCalibration(&dummy_sys, &dummy_gyro, &accel, &magn);
  
  
      FIRFilter_Init(&accx_filter, filter_coeff, filter_buff_accx, filter_order);//setup FIR filter for x accelaration
      FIRFilter_Init(&accy_filter, filter_coeff, filter_buff_accy, filter_order);//for y
      FIRFilter_Init(&accz_filter, filter_coeff, filter_buff_accz, filter_order);//for z
}

int landing_detection(int landing_switch){
    bool flag = landing_acc_reading_is_stable();
    if(flag)
        Serial.println("landed !");
    return 0;
}

static void landing_update_acceleration()
{
    bno.getEvent(&accel_data, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    old_accx = accx;
    old_accy = accy;
    old_accz = accz;
    accx = FIRFilter_Update (&accx_filter, accel_data.acceleration.x);
    accy = FIRFilter_Update (&accy_filter, accel_data.acceleration.y);
    accz = FIRFilter_Update (&accz_filter, accel_data.acceleration.z);
}

static bool landing_data_within_range (float old, float current)
{
    return (abs(current - old) <= ERROR_THRESHOLD);
}

static bool landing_acc_reading_is_stable(void)
{
    int count = 0;
    //free running for some time to populate the filter signal buffer
    while(count < 15)
    {
      landing_update_acceleration();
      //Serial.println(accx);
      //Serial.println(accy);
      //Serial.println(accz);
      count++;
    }
    count = 0;
    while(count < 20)
    {
      landing_update_acceleration();
      bool det1 = landing_data_within_range(old_accx,accx);
      bool det2 = landing_data_within_range(old_accy,accy);
      bool det3 = landing_data_within_range(old_accz,accz);
      Serial.println(det1);
      Serial.println(det2);
      Serial.println(det3);
      if(det1 && det2 && det3)
          count++; 
      else 
          return false;

      delay(50);
    }
    return true;
}

static void landing_update_magnetics()
{
    bno.getEvent(&mag_data, Adafruit_BNO055::VECTOR_MAGNETOMETER);
    
    magx = mag_data.magnetic.x;
    magy = mag_data.magnetic.y;
    magz = mag_data.magnetic.z;
}
