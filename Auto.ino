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
#include "Modules/BluetoothControl.h"

int runningMode = 0;          // current mode of the car
int stopSpeed = 0;            // speed at which the car was stopped by the ultrasone sensor
bool gyroFlag = false;        // has the angle been reached yet?
bool gyroHasStopped = false;  // has the gyro already stopped the car?

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
  bluetooth_init(&runningMode);

  #if CALIBRATE_LINETRACKER
  qtr_calibrate();
  #endif
  qtr_init();

  Logger::info("End of setup");
  Logger::info(3);
  delay(1000);
  Logger::info(2);
  delay(1000);
  Logger::info(1);
  delay(1000);
  Logger::info("Enabled motor");
  motor_controlMotor(MOTOR_FORWARD, 180);

}

void loop()
{
  bluetooth_loop();

  float distance = us_getDistance();
  if(distance != 0.0 && distance < 20.0 && motor_getCurrentSpeed() != 0)
  {
    stopSpeed = motor_getCurrentSpeed();
    motor_stop();
    Logger::warning("Stopped car because object is too close!");
  }
  else if(stopSpeed != 0 && distance == 0.0)
  {
    Logger::info("Object removed, car is moving again");
    motor_controlMotor(MOTOR_FORWARD, stopSpeed);
    stopSpeed = 0;
  }

  if(runningMode == 0)
  {
    float gyroAngle = gyro_getAngle();

    if(gyroAngle < -10)
    {
      gyroFlag = true;

    }else if(gyroFlag && gyroAngle > -4)
    {
      if(!gyroHasStopped)
      {
        Logger::info("Reached end of track");
        motor_stop();
        gyroHasStopped = true;
      }

    }

    motor_controlServo(qtr_getSteerAmount());
  }

}
