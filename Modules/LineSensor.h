/*
 * @author Thomas Gortemaker
 * @date 19-05-18
 * @description functions for the qtra linetracker
 */
#ifndef LineSensor_h
#define LineSensor_h

#include "Arduino.h"
#include <QTRSensors.h>
#include "../Util/Logger.h"

#define QTR_NUM_SENSORS             6  // number of sensors used
#define QTR_NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define QTR_EMITTER_PIN             48  // emitter is controlled by digital pin 2
#define QTR_MAX_VALUE               (QTR_NUM_SENSORS - 1) * 1000 // puts the value of the middle of the line at 3500
#define QTR_UNDER_LIMIT             200
#define QTR_UPPER_LIMIT             4000

QTRSensorsAnalog qtra((unsigned char[]) {A9, A10, A11, A12, A13, A14},
  QTR_NUM_SENSORS, QTR_NUM_SAMPLES_PER_SENSOR, QTR_EMITTER_PIN);
unsigned int qtr_sensorValues[QTR_NUM_SENSORS];
int qtr_lastPosition;

/**
 * Initialize the linetracker sensors
 */
void qtr_init()
{
  Logger::info("Intializing linetracker");
  qtr_lastPosition = qtra.readLine(qtr_sensorValues);
}

/**
 * Calibrate the linetrackers sensors
 */
void qtr_calibrate()
{
  Logger::info("Calibrating linetracker");
  Logger::info("Please move the line from left to right underneath the car.");
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtra.calibrate();       // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)
  }
  Logger::info("Finished calibrating linetracker");
}

/**
 * Get the steering direction => 0-50 left, 50-100 right
 * @return int
 */
int qtr_getSteerAmount()
{
  unsigned int position = qtra.readLine(qtr_sensorValues); //MIDDLE_LINE zorgt ervoor dat het midden van de lijn wordt aangegeven met 0 i.p.v. 3500.
  return map(position, QTR_UNDER_LIMIT, QTR_UPPER_LIMIT, 0, 100);
}

#endif
