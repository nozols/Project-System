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

#define MOTOR_SERVO_PIN 11
#define MOTOR_FORWARD LOW
#define MOTOR_BACKWARD HIGH
#define MOTOR_DIRECTION_PIN 2
#define MOTOR_SPEED_PIN 3
#define MOTOR_MAX_STEER_ANGLE 40

Servo motor_steerServo;

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

void motor_controlMotor(uint8_t direction, uint8_t speed)
{
  digitalWrite(MOTOR_DIRECTION_PIN, direction);
  analogWrite(MOTOR_SPEED_PIN, speed);
}

void motor_stop()
{
  analogWrite(MOTOR_SPEED_PIN, 0);
}

/**
 * Control the steering servo
 * TODO check min and max steering angle
 * @param int turnPercentage 0-50 left, 50-100 right
 */
void motor_controlServo(int turnPercentage)
{
  int turnServoValue = map(turnPercentage, 0, 100, 90 - MOTOR_MAX_STEER_ANGLE, 90 + MOTOR_MAX_STEER_ANGLE);//convert procents to Servo value
  motor_steerServo.write(turnServoValue);
}

#endif
