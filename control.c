#include "arduino.h"
#include <Servo.h>

#define SERVOPIN 2

#define MOTORPIN 3

Servo steerServo;

void controlSetup()
{
  steerServo.attach(SERVOPIN);
  pinMode(MOTORPIN, OUTPUT);
}

void controlServo(int turnPercentage)
{
  int turnServoValue = map(turnPercentage, 0, 100, 0, 180);//convert procents to Servo value
  steerServo.write(turnServoValue);
}

void controlMotor(int speedPercentage)
{
  int speedPWMValue = map(speedPercentage, 0, 100, 0, 255);//convert procents to PWM value
  analogWrite(MOTORPIN, speedPWMValue);
}

