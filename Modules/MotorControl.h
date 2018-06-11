/*
 * @author Peter van Breugel
 * @date 15-05-18
 * @description functions for the motor module
 */
#ifndef MotorControl_h
#define MotorControl_h

#include "Arduino.h"
#include "../Util/Logger.h"
#include <Servo.h>

#define MOTOR_SERVO_PIN             22    // pin for servo
#define MOTOR_FORWARD               LOW   // mode for going forward
#define MOTOR_BACKWARD              HIGH  // mode for going backwards
#define MOTOR_DIRECTION_PIN         2     // pin for direction
#define MOTOR_SPEED_PIN             3     // pin for speed (pwm)
#define MOTOR_MAX_STEER_ANGLE       50    // maximum angle for servo

Servo motor_steerServo;

int motor_currentSpeed = 0;

/**
 * Initialize motor modules
 */
void motor_init()
{
  Logger::info("Initializing servo and motor");
  motor_steerServo.attach(MOTOR_SERVO_PIN);
  pinMode(MOTOR_DIRECTION_PIN, OUTPUT);
  pinMode(MOTOR_SPEED_PIN, OUTPUT);
  digitalWrite(MOTOR_DIRECTION_PIN, MOTOR_FORWARD);
  analogWrite(MOTOR_SPEED_PIN, 0);
}

/**
 * Control the main motor
 * @param direction uint8_t - the direction (MOTOR_FORWARD or MOTOR_BACKWARD)
 * @param speed uint8_t - speed of the motor between 0 and 255 (pwm)
 */
void motor_controlMotor(uint8_t direction, uint8_t speed)
{
  motor_currentSpeed = speed;
  digitalWrite(MOTOR_DIRECTION_PIN, direction);
  analogWrite(MOTOR_SPEED_PIN, speed);
}

/**
 * Stop the motor
 */
void motor_stop()
{
  motor_controlMotor(MOTOR_FORWARD, 0);
}

/**
 * Control the steering servo
 * @param int turnPercentage 0-50 left, 50-100 right
 */
void motor_controlServo(int turnPercentage)
{
  int turnServoValue = map(turnPercentage, 0, 100, 90 - MOTOR_MAX_STEER_ANGLE, 90 + MOTOR_MAX_STEER_ANGLE);//convert procents to Servo value
  motor_steerServo.write(turnServoValue);
}

/**
 * Returns the current speed of the motor
 */
int motor_getCurrentSpeed()
{
  return motor_currentSpeed;
}

#endif
