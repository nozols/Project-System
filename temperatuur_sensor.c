#include "arduino.h"

#define MAXTEMP 100
#define TEMPPIN A9

bool tempExceededMax = 0;

void tempSensSetup()
{
  pinMode(TEMPPIN, INPUT);

  if(analogRead(TEMPPIN) > MAXTEMP)
  {
    tempExceededMax = 1;
  }
}

void tempSensLoop()
{
  if(analogRead(TEMPPIN) > MAXTEMP)
  {
    tempExceededMax = 1;
  }
  else
  {
    tempExceededMax = 0;
  }
}

