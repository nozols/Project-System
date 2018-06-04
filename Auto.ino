#define CALIBRATE_GYRO false
#define CALIBRATE_LINETRACKER false
#define LOGGER_SD_ENABLED false
#define LOGGER_BLUETOOTH_ENALBED false

#include <Wire.h>
#include <Servo.h>
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
  gyro.setGyroOffsets(-4.00, -1.2, -1.2);
  Logger::info("Started gyro");
  us_init();
  motor_init();
  temp_init();

  #if CALIBRATE_GYRO
  Logger::info("Calibrating gyro. Do not move car.");
  gyro.calcGyroOffsets(true);
  Logger::info("Finished calibration.");
  #endif
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
