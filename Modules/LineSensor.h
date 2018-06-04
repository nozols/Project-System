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

#define QTR_NUM_SENSORS             8  // number of sensors used
#define QTR_NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define QTR_EMITTER_PIN             48  // emitter is controlled by digital pin 2
#define QTR_MIDDLE_LINE             3500 // puts the value of the middle of the line at 3500

QTRSensorsAnalog qtra((unsigned char[]) {8, 9, 10, 11, 12, 13, 14, 15},
  QTR_NUM_SENSORS, QTR_NUM_SAMPLES_PER_SENSOR, QTR_EMITTER_PIN);
unsigned int qtr_sensorValues[QTR_NUM_SENSORS];
int qtr_lastPosition;

/**
 * Initialize the linetracker sensors
 */
void qtr_init()
{
  Logger::info("Intializing linetracker");
  qtr_lastPosition = qtra.readLine(qtr_sensorValues) - QTR_MIDDLE_LINE;
}

/**
 * Calibrate the linetrackers sensors
 */
void qtr_calibrate()
{
  Logger::info("Calibrating linetracker");
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
  int position = qtra.readLine(qtr_sensorValues) - QTR_MIDDLE_LINE; //MIDDLE_LINE zorgt ervoor dat het midden van de lijn wordt aangegeven met 0 i.p.v. 3500.
  int difference = position - qtr_lastPosition;
  qtr_lastPosition = position;
  Logger::info(position);

  if (((position > 0) && (difference > 0))  || ((position) < 0 && (difference < 0)))
  {
    return position / 70 + 50; //levert een waarde van 0 tot 100 op. (<50 = links & >50 = rechts)
  }
  else if (position < (difference * -1))
  {
    return difference / 70 + 50; //levert een waarde van 0 tot 100 op. (<50 = links & >50 = rechts)
  }
}

#endif
