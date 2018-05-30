/*
 * @author Niels de Boer
 * @date 15-05-18
 * @description functions for the HC-SR04 ultrasone sensor
 */
#ifndef UltrasoneSensor_h
#define UltrasoneSensor_h

#include "Arduino.h"

#define US_TRIG_PIN 12
#define US_ECHO_PIN 13
#define US_PULSE_IN_TIMEOUT 2000

/**
 * Initialize the pins for the ultrasone sensor
 */
void us_initialize(){
  pinMode(US_TRIG_PIN, OUTPUT);
  pinMode(US_ECHO_PIN, INPUT);
}

/**
 * Get the pulseduration
 * @returns unsigned long pulseduration
 */
unsigned long us_getDuration()
{
  digitalWrite(US_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(US_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_TRIG_PIN, LOW);

  return pulseIn(US_ECHO_PIN, HIGH, US_PULSE_IN_TIMEOUT);
}

/**
 * Get the distance in cm from the object. 0 is invalid
 * @returns float distance in cm
 */
float us_getDistance()
{
  return (us_getDuration() / 2) / 29.1;
}

#endif
