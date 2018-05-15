#define LOGGER_SD_ENABLED false

#include "control.c"
#include "Util\Logger.h"
#include "Util\SDcard.h"
#include "MPU6050\src\MPU6050_tockn.h"
#include <Wire.h>

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
  gyro.calcGyroOffsets(false);
  Logger::info("Finished calibration.");


  if(!canStart)
  {
    Logger::warning("Could not start the car!");
    while(1){}  // Infinte loop, so void loop() doesn't get called
  }
  Logger::error("Test error message");
  Logger::warning("Test warning message");
  Logger::debug("Test debug message");
  Logger::info("Test info message");

    for(byte i = 0; i <= 200; i++){
      Logger::data("testgraph", i);
    }


    for(int i = 0; i <= 200; i++){
      Logger::data("Moargraph", sin(i * (3.1415926 / 180)));
    }

    controlSetup();
}

void loop()
{
  gyro.update();
  Logger::data("gyro_x", gyro.getAngleX());
  Logger::data("gyro_y", gyro.getAngleY());
  Logger::data("gyro_z", gyro.getAngleZ());
  Logger::data("temp", gyro.getTemp());

  controlServo();//<-----moet nog een percentage meegegeven worden!
  controlMotor();//<-----moet nog een percentage meegegeven worden!
}
