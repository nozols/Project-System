/*
 * @author Peter van Breugel
 * @date 20-05-18
 * @description functions for the temperature sensors
 * @notimplemented
 */
#ifndef TemperatureSensor_h
#define TemperatureSensor_h

#include "Arduino.h"
#include "../Util/Logger.h"

#define TEMP_PIN A7
#define TEMP_MAX 100

/**
 * Initialize temperature sensors
 */
void temp_init()
{
  Logger::info("Initializing temperature sensor");
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
