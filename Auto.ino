#define CALIBRATE_SENSORS false
#define LOGGER_SD_ENABLED false

#include <Wire.h>
#include <Servo.h>
#include <QTRSensors.h>
#include "Util/Logger.h"
#include "MPU6050/src/MPU6050_tockn.h"
#include "Sensors/UltrasoneSensor.h"
#include "Sensors/LineSensor.h"
#include "Sensors/TemperatureSensor.h"

MPU6050 gyro(Wire);
Servo steerServo;
Servo motorServo;

#define SERVOPIN 11
#define MOTORPIN 23

bool canStart = true;

void setup()
{
  Serial.begin(9600);
  Logger::init();
  Logger::info("Starting engines...");
  Wire.begin();
  Logger::info("Started wire.h");
  gyro.begin();
  #if CALIBRATE_SENSORS
  Logger::info("Calibrating gyro. Do not move car.");
  gyro.calcGyroOffsets(false);
  Logger::info("Finished calibration.");
  #endif
  Logger::info("Initializing ultrasone sensor");
  us_initialize();
  Logger::info("Finished initializing ultrasone sensor");

  Logger::info("Initializing steering servo");
  steerServo.attach(SERVOPIN);
  motorServo.attach(MOTORPIN);
  temp_init();
  Logger::info("Finished initializing steering servo");

  #if CALIBRATE_SENSORS
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

/**
 * Control the steering servo
 * TODO check min and max steering angle
 * @param int turnPercentage 0-50 left, 50-100 right
 */
void controlServo(int turnPercentage)
{
  int turnServoValue = map(turnPercentage, 0, 100, 0, 180);//convert procents to Servo value
  steerServo.write(turnServoValue);
}

/**
 * Control the main motor
 * @param int speedPercentage 0 is stop, 100 full throttle
 */
void controlMotor(int speedPercentage)
{
  if(temp_isTemperatureTooHigh())
  {//check if the temperature is too high
    speedPercentage = map(speedPercentage, 0, 100, 0, 50);//reduce the speed when to hot
  }

  int speedPWMValue = map(speedPercentage, 0, 100, 0, 255);//convert procents to PWM value
  //analogWrite(MOTORPIN, speedPWMValue);
  Logger::info(speedPWMValue);
  motorServo.write(speedPWMValue);
}
