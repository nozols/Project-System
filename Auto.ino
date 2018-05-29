#define LOGGER_SD_ENABLED false

#include <Wire.h>
#include <Servo.h>
#include "Util/Logger.h"
#include "MPU6050/src/MPU6050_tockn.h"
#include "ultrasone_sensor.c"

MPU6050 gyro(Wire);
Servo steerServo;

#define SERVOPIN 2
#define MOTORPIN 3
#define TEMPPIN A9

#define MAXTEMP 100

bool canStart = true;

void setup()
{
  Serial.begin(9600);
  Logger::init();
  Logger::info("Starting engines...");
  Wire.begin();
  Logger::info("Started wire.h");
  gyro.begin();
  Logger::info("Calibrating gyro. Do not move car.");
  gyro.calcGyroOffsets(false);
  Logger::info("Finished calibration.");
  Logger::info("Initializing ultrasone sensor");
  us_initialize();
  Logger::info("Finished initializing ultrasone sensor");

  Logger::info("Initializing steering servo");
  steerServo.attach(SERVOPIN);
  pinMode(MOTORPIN, OUTPUT);
  pinMode(TEMPPIN, INPUT);
  Logger::info("Finished initializing steering servo");

  if(!canStart)
  {
    Logger::error("Could not start the car!");
    while(1){}  // Infinte loop, so void loop() doesn't get called
  }

  Logger::info("End of setup");


}
int i = 0;
void loop()
{
  i++;
  //controlServo(0);//<-----moet nog een percentage meegegeven worden!
  //controlMotor(0);//<-----moet nog een percentage meegegeven worden!
  gyro.update();
  Logger::data("GYRO", gyro.getAngleX());
  Logger::data("US", us_getDistance());
  tempSensCheck();

  controlServo(i);//<-----moet nog een goed percentage meegegeven worden!
  controlMotor(50);//<-----moet nog een goed percentage meegegeven worden!
  if(i >= 100){ i = 0;}
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
  if(tempSensCheck())
  {//check if the temperature is too high
    speedPercentage = map(speedPercentage, 0, 100, 0, 50);//reduce the speed when to hot
  }

  int speedPWMValue = map(speedPercentage, 0, 100, 0, 255);//convert procents to PWM value
  analogWrite(MOTORPIN, speedPWMValue);
}

bool tempSensCheck()
{
  return analogRead(TEMPPIN) > MAXTEMP;
}
