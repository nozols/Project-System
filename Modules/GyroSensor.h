/*
 * @author Niels de Boer
 * @date 05-06-18
 * @description Functions for the gyroscope
 */
#ifndef GyroSensor_h
#define GyroSensor_h

#include <Wire.h>
#include "../Util/Logger.h"
#include "../MPU6050/src/MPU6050_tockn.h"

MPU6050 gyro(Wire);

float gyro_startAngle = 0;  // the angle at which the gyro was at initialization

/**
 * @returns get the angle of the gyro
 */
float gyro_getAngle()
{
  gyro.update();
  return gyro.getAngleY() - gyro_startAngle;
}

/**
 * Initialize the gyro
 * @param calibrate bool - should the sensor be calibrated?
 */
void gyro_init(bool calibrate)
{
  Logger::info("Initializing gyro");
  gyro.begin();
  gyro.setGyroOffsets(-1.88, -1.72, -1.72);

  if(calibrate)
  {
    gyro.calcGyroOffsets(true);
  }
  delay(1000);
  gyro_startAngle = gyro_getAngle();
  Logger::info(gyro_startAngle);
}

#endif
