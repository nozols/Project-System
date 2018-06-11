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

#define QTR_WHITE_LINE              true   // is the line white? (false => line is black)
#define QTR_NUM_SENSORS             6      // number of sensors used
#define QTR_NUM_SAMPLES_PER_SENSOR  4      // average 4 analog samples per sensor reading
#define QTR_EMITTER_PIN             48     // emitter is controlled by digital pin 2
#define QTR_MAX_VALUE               (QTR_NUM_SENSORS - 1) * 1000   // puts the value of the middle of the line at 3500
#define QTR_OFF_TRACK               100.0  // Value that is returned by qtr_avg when car is of the track
#define QTR_SENSOR_THRESHOLD        150    // threshold that is used to determine how white/black a line has to be to be considered valid

QTRSensorsAnalog qtra((unsigned char[]) {A9, A10, A11, A12, A13, A14},
  QTR_NUM_SENSORS, QTR_NUM_SAMPLES_PER_SENSOR, QTR_EMITTER_PIN);
unsigned int qtr_sensorValues[QTR_NUM_SENSORS];
int qtr_lastPosition = 0;    // stores the last position of the line

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
 * Get the position of the line
 * @returns float line position
 */
float qtr_avg()
{
  int totalIndexes = 0;
  int indexSum = 0;
  for(int index = 0; index < QTR_NUM_SENSORS; index++)
  {
    if(qtr_sensorValues[index] < QTR_SENSOR_THRESHOLD)
    {
      indexSum += index;
      totalIndexes++;
    }
  }

  return totalIndexes != 0 ? indexSum / totalIndexes : QTR_OFF_TRACK;
}

/**
 * Get the steering direction => 0-50 left, 50-100 right
 * @return int
 */
int qtr_getSteerAmount()
{
  qtra.readLine(qtr_sensorValues, QTR_WHITE_LINE);

  int lowestIndex = qtr_avg();

  if(lowestIndex == QTR_OFF_TRACK)
  {
    Logger::info("OFF TRACK");
    lowestIndex = qtr_lastPosition;
  }else{
    qtr_lastPosition = lowestIndex;
  }

  return map(lowestIndex, 0, QTR_NUM_SENSORS, 0, 100);
}

#endif
