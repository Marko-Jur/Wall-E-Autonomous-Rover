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
const float ACC_ERROR_THRESHOLD = 0.02;
const float MAG_ERROR_THRESHOLD = 0.40;

Servo LANDING_SERVO;

//static variables
static  sensors_event_t accel_data;
static float accx = 0;
static float accy = 0;
static float accz = 0;
static float old_accx = 0;
static float old_accy = 0;
static float old_accz = 0;

static  sensors_event_t mag_data;
static float magx = 0;
static float magy = 0;
static float magz = 0;
static float old_magx = 0;
static float old_magy = 0;
static float old_magz = 0;

//filter-related info
static FIRFilter accx_filter;
static FIRFilter accy_filter;
static FIRFilter accz_filter;
static FIRFilter magx_filter;
static FIRFilter magy_filter;
static FIRFilter magz_filter;



const static uint8_t filter_order = 15;
static float filter_coeff[15] = {-0.005071,0.011302,0.016822,-0.029808,-0.065989,0.051410,
0.302127, 0.438786,0.302127,0.051410, -0.065989,-0.029808,0.016822,0.011303,-0.005071}; //filter coeficent for FIR LP filter
static float filter_buff_accx[15] = {0};
static float filter_buff_accy[15] = {0};
static float filter_buff_accz[15] = {0};
static float filter_buff_magx[15] = {0};
static float filter_buff_magy[15] = {0};
static float filter_buff_magz[15] = {0};

//bno instance
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28); 

//static functions
static bool landing_acc_within_range (float old, float current);
static bool landing_mag_within_range (float old, float current);
static void landing_update_acceleration(void);
static bool landing_acc_reading_is_stable(void);
static void landing_update_magnetics(void);
static bool landing_mag_reading_is_stable(void);

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
      FIRFilter_Init(&magx_filter, filter_coeff, filter_buff_magx, filter_order);//for x magnetometer reading
      FIRFilter_Init(&magy_filter, filter_coeff, filter_buff_magy, filter_order);//for y 
      FIRFilter_Init(&magz_filter, filter_coeff, filter_buff_magz, filter_order);//for z
}

int landing_detection(int landing_switch){
    bool flag1 = landing_acc_reading_is_stable();
    bool flag2 = landing_mag_reading_is_stable();
    
    if(flag1 && flag2)
        Serial.println("Landed !");
        
    return 0;
}

static bool landing_acc_within_range (float old, float current)
{
    return (abs(current - old) <= ACC_ERROR_THRESHOLD);
}

static bool landing_mag_within_range (float old, float current)
{
    return (abs(current - old) <= MAG_ERROR_THRESHOLD);
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
      bool det1 = landing_acc_within_range(old_accx,accx);
      bool det2 = landing_acc_within_range(old_accy,accy);
      bool det3 = landing_acc_within_range(old_accz,accz);
      //Serial.println(det1);
      //Serial.println(det2);
      //Serial.println(det3);
      if(det1 && det2 && det3)
          count++; 
      else 
          return false;

      delay(50);
    }
    return true;
}

static bool landing_mag_reading_is_stable(void)
{
    int count = 0;
    //free running for some time to populate the filter signal buffer
    while(count < 15)
    {
      landing_update_magnetics();
      count++;
    }
    
    count = 0;
    
    while(count < 15)
    {
      landing_update_magnetics();
      bool det1 = landing_mag_within_range(old_magx,magx);
      bool det2 = landing_mag_within_range(old_magy,magy);
      bool det3 = landing_mag_within_range(old_magz,magz);
      /*Serial.println("magx   magy   magz");
      Serial.print(magx);
      Serial.print("   ");
      Serial.print(magy);
      Serial.print("   ");
      Serial.println(magz);*/

      //Serial.println(det1);
      //Serial.println(det2);
      //Serial.println(det3);
      
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
    old_magx = magx;
    old_magy = magy;
    old_magz = magz;
    magx = FIRFilter_Update (&magx_filter, mag_data.magnetic.x);
    magy = FIRFilter_Update (&magy_filter, mag_data.magnetic.y);
    magz = FIRFilter_Update (&magz_filter, mag_data.magnetic.z);
}
