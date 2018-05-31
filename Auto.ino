#define CALIBRATE_SENSORS false
#define LOGGER_SD_ENABLED false

#include <Wire.h>
#include "Util/Logger.h"
#include "MPU6050/src/MPU6050_tockn.h"
#include "Modules/UltrasoneSensor.h"
#include "Modules/LineSensor.h"
#include "Modules/TemperatureSensor.h"
#include "Modules/MotorControl.h"

MPU6050 gyro(Wire);

bool canStart = true;

void setup()
{
  Serial.begin(9600);
  Logger::init();
  Logger::info("Starting engines...");
  Wire.begin();
  Logger::info("Started wire");
  gyro.begin();
  Logger::info("Started gyro");
  us_init();
  motor_init();
  temp_init();

  #if CALIBRATE_SENSORS
  Logger::info("Calibrating gyro. Do not move car.");
  gyro.calcGyroOffsets(false);
  Logger::info("Finished calibration.");

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

void loop()
{
  Logger::data("us", us_getDistance());
  Logger::data("gx", gyro.getAngleX());
  Logger::data("temp", temp_isTemperatureTooHigh());
  Logger::data("steer", qtr_getSteerAmount());
}
