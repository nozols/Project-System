#define LOGGER_SD_ENABLED false
#include "Util\Logger.h"
//#include "Util\SDcard.h"
#include "MPU6050\src\MPU6050_tockn.h"

//libraries
#include <Wire.h>
#include <Servo.h>

#include "ultrasone_sensor.c"

Servo steerServo;

MPU6050 gyro(Wire);

#define SERVOPIN 2
#define MOTORPIN 3
#define TEMPPIN A9

#define MAXTEMP 100

bool canStart = true;
bool tempExceededMax = false;

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

  //I/O declaration
  steerServo.attach(SERVOPIN);
  pinMode(MOTORPIN, OUTPUT);
  pinMode(TEMPPIN, INPUT);
  
  if(!canStart)
  {
    Logger::error("Could not start the car!");
    while(1){}  // Infinte loop, so void loop() doesn't get called
  }
  Logger::info("End of setup");

  
}

void loop()
{
  tempSensCheck();
  
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
  if(tempExceededMax == true)
  {//check if the temperature is too high
    speedPercentage = map(speedPercentage, 0, 100, 0, 50);//reduce the speed when to hot
  }
  
  int speedPWMValue = map(speedPercentage, 0, 100, 0, 255);//convert procents to PWM value
  analogWrite(MOTORPIN, speedPWMValue);
}

void tempSensCheck()
{
  if(analogRead(TEMPPIN) > MAXTEMP)
  {
    tempExceededMax = true;
  }
  else
  {
    tempExceededMax = false;
  }
}
