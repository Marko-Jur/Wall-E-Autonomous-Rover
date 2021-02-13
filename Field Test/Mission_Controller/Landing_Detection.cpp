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
#include "Nav_System.h"

//-------------------------------------------------------------------------------------------------
// constants
//-------------------------------------------------------------------------------------------------

//servo constants
const static int SERVO_DETACH = 2100;
const static int SERVO_ENGAGED = 1200;

//offsets for bno055 check
const static float ACC_ERROR_THRESHOLD = 0.02; //data reading is considered to be stable when fluctuation is within these ranges
const static float MAG_ERROR_THRESHOLD = 0.40; 
const static float ALT_ERROR_THRESHOLD = 1.00; // a relatively large range to account for errors

//constants for altitude check
const static float SEALEVELPRESSURE_HPA = 1021.9; // sea level pressure of the day to be set on the competition day
const static float DESTINATION_ALTITUDE = 190.0;     // the altitude of destination
const static float START_CHECKING_ALTITUDE = 189.0;  // the altitude to start checking the sensors

const static uint8_t filter_order = 15;

//-------------------------------------------------------------------------------------------------
// static instances
//-------------------------------------------------------------------------------------------------

Servo landing_servo;
Adafruit_BMP3XX bmp;
//A bno is already there in Nav_System.h 

//-------------------------------------------------------------------------------------------------
// static variables
//-------------------------------------------------------------------------------------------------

//bno055 sensor-related data
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

//bmp388 altitude
static float alt_data = 0;

//filter-related info
static FIRFilter accx_filter;
static FIRFilter accy_filter;
static FIRFilter accz_filter;
static FIRFilter magx_filter;
static FIRFilter magy_filter;
static FIRFilter magz_filter;
static float filter_coeff[15] = {-0.005071,0.011302,0.016822,-0.029808,-0.065989,0.051410,
0.302127, 0.438786,0.302127,0.051410, -0.065989,-0.029808,0.016822,0.011303,-0.005071}; //filter coeficent for FIR LP filter
static float filter_buff_accx[15] = {0};
static float filter_buff_accy[15] = {0};
static float filter_buff_accz[15] = {0};
static float filter_buff_magx[15] = {0};
static float filter_buff_magy[15] = {0};
static float filter_buff_magz[15] = {0};


//static functions
static bool landing_within_range (float old, float current, float range);
static void landing_update_acceleration (void);
static bool landing_acc_reading_is_stable (void);
static void landing_update_magnetics (void);
static bool landing_mag_reading_is_stable (void);
static void landing_update_altitude (void);

//global functions
void landing_setup(void);
bool landing_detection(void);
bool landing_start_checking(void);

void landing_setup(){
  
      Serial.begin(115200);

      landing_servo.attach(SERVO); // attach pin 7
      landing_servo.write(SERVO_ENGAGED);

      //initialize both modules
      if (!bno.begin())
      {
        for(;;){Serial.println("No BNO055 detected");} //shoots the error if disconnected
      }

      if (!bmp.begin_SPI(BMP_CS))
      {
        for(;;){Serial.println("No BMP388 detected");}
      }

      //initialize variable and filter buffer for bno055 operations

      uint8_t sys = 0, gyro = 0, accel = 0, magn = 0; //other calibration flags are dummy as we are only using
                                                      //accelerometer and compass for landing
      bno.getCalibration(&sys, &gyro, &accel, &magn); //get calibration

      FIRFilter_Init(&accx_filter, filter_coeff, filter_buff_accx, filter_order);//setup FIR filter for x accelaration
      FIRFilter_Init(&accy_filter, filter_coeff, filter_buff_accy, filter_order);//for y
      FIRFilter_Init(&accz_filter, filter_coeff, filter_buff_accz, filter_order);//for z
      FIRFilter_Init(&magx_filter, filter_coeff, filter_buff_magx, filter_order);//for x magnetometer reading
      FIRFilter_Init(&magy_filter, filter_coeff, filter_buff_magy, filter_order);//for y 
      FIRFilter_Init(&magz_filter, filter_coeff, filter_buff_magz, filter_order);//for z

      //initialize the bmp sensor according to datasheet recommendations
      bmp.setPressureOversampling(BMP3_OVERSAMPLING_2X);
      bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_DISABLE);
      bmp.setOutputDataRate(BMP3_ODR_100_HZ);
      
}

bool landing_detection(){

    bool flag1, flag2, flag3;

      //acc and mag stability check
    flag1 = landing_acc_reading_is_stable();
    flag2 = landing_mag_reading_is_stable();

    Serial.println(alt_data);
      //altitude matching check
    landing_update_altitude();
    flag3 = landing_within_range(alt_data, DESTINATION_ALTITUDE, ALT_ERROR_THRESHOLD);
    
    
    if(flag1 && flag2 && flag3)
    {
        Serial.println("Landed !");
        landing_servo.write(SERVO_DETACH); 
    }
        
    return 0;
}

//checks whether we should start the landing detection algorithm
bool landing_start_checking()
{
    landing_update_altitude();
    if(alt_data > 200)
      return true;
    else
      return false;
}

static void landing_update_acceleration()
{
    bno.getEvent(&accel_data, Adafruit_BNO055::VECTOR_ACCELEROMETER); //update reading
    old_accx = accx;
    old_accy = accy;
    old_accz = accz;
    accx = FIRFilter_Update (&accx_filter, accel_data.acceleration.x); //filter the signals
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
      count++;
    }
    count = 0;
    while(count < 20)
    {
      landing_update_acceleration();
      bool det1 = landing_within_range(old_accx,accx,ACC_ERROR_THRESHOLD); //filter the signal
      bool det2 = landing_within_range(old_accy,accy,ACC_ERROR_THRESHOLD);
      bool det3 = landing_within_range(old_accz,accz,ACC_ERROR_THRESHOLD);

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
    
    while(count < 20)
    {
      landing_update_magnetics();
      bool det1 = landing_within_range(old_magx,magx,MAG_ERROR_THRESHOLD);
      bool det2 = landing_within_range(old_magy,magy,MAG_ERROR_THRESHOLD);
      bool det3 = landing_within_range(old_magz,magz,MAG_ERROR_THRESHOLD);
      
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

static void landing_update_altitude()
{
    bool det = bmp.performReading();
    if(det)
       alt_data = bmp.readAltitude(SEALEVELPRESSURE_HPA); // data in meters
}

static bool landing_within_range (float old, float current, float range)
{
    return (abs(current - old) <= range);
}
