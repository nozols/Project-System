#define CALIBRATE_GYRO true
#define CALIBRATE_LINETRACKER true
#define LOGGER_SD_ENABLED true
#define LOGGER_BLUETOOTH_ENABLED true

#include "Util/Logger.h"
#include "MPU6050/src/MPU6050_tockn.h"
#include "Modules/UltrasoneSensor.h"
#include "Modules/LineSensor.h"
#include "Modules/TemperatureSensor.h"
#include "Modules/GyroSensor.h"
#include "Modules/MotorControl.h"

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

  motor_controlMotor(MOTOR_FORWARD, 180);
  Logger::info("End of setup");
}

String str = "";
int stopSpeed = 0;
void loop()
{

  float distance = us_getDistance();
  if(distance != 0.0 && distance < 20.0 && motor_getCurrentSpeed() != 0)
  {
    stopSpeed = motor_getCurrentSpeed();
    motor_stop();
    Logger::debug(distance);
    Logger::warning("Stopped car because object is too close!");
  }
  else if(stopSpeed != 0 && distance == 0.0)
  {
    Logger::info("Object removed, car is moving again");
    motor_controlMotor(MOTOR_FORWARD, stopSpeed);
    stopSpeed = 0;
  }

  //gyro.update();
//  Logger::data("us", us_getDistance());
  //Logger::data("gx", gyro_getAngle());
  //Logger::data("temp", temp_isTemperatureTooHigh());
  //Logger::data("steer", qtr_getSteerAmount());
  motor_controlServo(qtr_getSteerAmount());
  while(Serial1.available() >= 1)
  {
    char read = Serial1.read();

    if(read == '\n'){
      motor_controlMotor(MOTOR_FORWARD, str.toInt());
      Logger::info(str.toInt());
      str = "";
    }else{
      str += read;
    }
  }
}
