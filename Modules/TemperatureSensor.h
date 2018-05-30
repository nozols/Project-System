/*
 * @author Peter van Breugel
 * @date 20-05-18
 * @description functions for the temperature sensors
 */
#ifndef TemperatureSensor_h
#define TemperatureSensor_h

#include "Arduino.h"

#define TEMP_PIN A9
#define TEMP_MAX 100

/**
 * Initialize temperature sensors
 */
void temp_init()
{
  pinMode(TEMP_PIN, INPUT);
}

/**
 * @return bool is the temperature of the motor too high?
 */
bool temp_isTemperatureTooHigh()
{
  return analogRead(TEMP_PIN) > TEMP_MAX;
}

#endif
