#define LOGGER_SD_ENABLED false


#include "Util\Logger.h"
#include "Util\SDcard.h"
#include "MPU6050\src\MPU6050_tockn.h"
#include <Wire.h>

#include "ultrasone_sensor.c"
#include "control.c"

MPU6050 gyro(Wire);

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


  if(!canStart)
  {
    Logger::error("Could not start the car!");
    while(1){}  // Infinte loop, so void loop() doesn't get called
  }
  controlSetup();
  Logger::info("End of setup");
}

void loop()
{
  controlServo();//<-----moet nog een percentage meegegeven worden!
  controlMotor();//<-----moet nog een percentage meegegeven worden!
  Logger::info(us_getDistance());
  delay(100);
}
