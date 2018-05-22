#define LOGGER_SD_ENABLED false
#include "Util\Logger.h"
//#include "Util\SDcard.h"
#include "MPU6050\src\MPU6050_tockn.h"
#include <Wire.h>
#include <Servo.h>

#include "ultrasone_sensor.c"
#include "temperatuur_sensor.c"

Servo steerServo;

MPU6050 gyro(Wire);

#define SERVOPIN 2
#define MOTORPIN 3

bool canStart = true;

unsigned long msgTime = 0;

void setup()
{
  Serial.begin(9600);
  Logger::init();
  Logger::info("Starting engines...");
  Wire.begin();
  Logger::info("Started wire.h");
  gyro.begin();
  Logger::info("Calibrating gyro. Do not move car.");
  //gyro.calcGyroOffsets(false);
  Logger::info("Finished calibration.");
  us_initialize();
  tempSensSetup();

  steerServo.attach(SERVOPIN);
  pinMode(MOTORPIN, OUTPUT);

  if(!canStart)
  {
    Logger::error("Could not start the car!");
    while(1){}  // Infinte loop, so void loop() doesn't get called
  }
  Logger::info("End of setup");
}

void loop()
{
  tempSensLoop();
  
  controlServo(50);//<-----moet nog een goed percentage meegegeven worden!
  controlMotor(50);//<-----moet nog een goed percentage meegegeven worden!
  Logger::info(us_getDistance());
  delay(100);
}

void controlServo(int turnPercentage)
{
  int turnServoValue = map(turnPercentage, 0, 100, 0, 180);//convert procents to Servo value
  steerServo.write(turnServoValue);
}


void controlMotor(int speedPercentage)
{
  if(tempExceededMax == 1)
  {
    speedPercentage = map(speedPercentage, 0, 100, 0, 50);//reduce the speed when to hot
  }
  
  int speedPWMValue = map(speedPercentage, 0, 100, 0, 255);//convert procents to PWM value
  analogWrite(MOTORPIN, speedPWMValue);
}
