#define CALIBRATE_GYRO false
#define CALIBRATE_LINETRACKER false
#define LOGGER_SD_ENABLED false
#define LOGGER_BLUETOOTH_ENALBED false

#include "Util/Logger.h"
#include "MPU6050/src/MPU6050_tockn.h"
#include "Modules/UltrasoneSensor.h"
#include "Modules/LineSensor.h"
#include "Modules/TemperatureSensor.h"
#include "Modules/GyroSensor.h"
#include "Modules/MotorControl.h"

bool canStart = true;

void setup()
{
  Serial.begin(9600);
  Logger::init();
  Logger::info("Starting engines...");
  Wire.begin();
  Logger::info("Started wire");

  gyro_init(CALIBRATE_GYRO);
  us_init();
  motor_init();
  temp_init();
  
  #if CALIBRATE_LINETRACKER
  qtr_calibrate();
  #endif
  qtr_init();


  if(!canStart)
  {
    Logger::error("Could not start the car!");
    while(1){}  // Infinte loop, so void loop() doesn't get called
  }

  Logger::info("End of setup");
}

String str = "";

void loop()
{
  //gyro.update();
  //Logger::data("us", us_getDistance());
  //Logger::data("gx", gyro.getAngleX());
  //Logger::data("temp", temp_isTemperatureTooHigh());
  //Logger::data("steer", qtr_getSteerAmount());
  //qtr_getSteerAmount();
  while(Serial.available() >= 1)
  {
    char read = Serial.read();

    if(read == '\n'){
      motor_controlMotor(MOTOR_FORWARD, str.toInt());
      Logger::info(str.toInt());
      str = "";
    }else{
      str += read;
    }
  }
}
