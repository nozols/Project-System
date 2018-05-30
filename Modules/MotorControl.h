/*
 * @author Peter van Breugel
 * @date 15-05-18
 * @description functions for the motor module
 */
#ifndef MotorControl_h
#define MotorControl_h

#include "Arduino.h"
#include <Servo.h>

#define MOTOR_SERVO_PIN 11
#define MOTOR_MOTOR_PIN 23

Servo motor_steerServo;
Servo motor_motorServo;

/**
 * Initialize motor modules
 */
void motor_init()
{
  motor_steerServo.attach(MOTOR_SERVO_PIN);
  motor_steerServo.attach(MOTOR_MOTOR_PIN);
}

/**
 * Control the steering servo
 * TODO check min and max steering angle
 * @param int turnPercentage 0-50 left, 50-100 right
 */
void motor_controlServo(int turnPercentage)
{
  int turnServoValue = map(turnPercentage, 0, 100, 0, 180);//convert procents to Servo value
  motor_steerServo.write(turnServoValue);
}

/**
 * Control the main motor
 * @param int speedPercentage 0 is stop, 100 full throttle
 */
void motor_controlMotor(int speedPercentage)
{
  if(temp_isTemperatureTooHigh())
  {//check if the temperature is too high
    speedPercentage = map(speedPercentage, 0, 100, 0, 50);//reduce the speed when to hot
  }

  int speedPWMValue = map(speedPercentage, 0, 100, 0, 255);//convert procents to PWM value
  //analogWrite(MOTOR_MOTOR_PIN, speedPWMValue);
  Logger::info(speedPWMValue);
  motor_motorServo.write(speedPWMValue);
}

#endif
