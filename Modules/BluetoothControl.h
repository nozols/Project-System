/*
 * @author Niels de Boer
 * @date 07-06-18
 * @description functions for the bluetooth functionality
 */
#ifndef BluetoothControl_h
#define BluetoothControl_h

#include "MotorControl.h"
#include "../Util/Logger.h"

// Command characters i.e. F100 => Forward command with 100 as parameter
#define BLUETOOTH_COMMAND_FORWARD       'F'
#define BLUETOOTH_COMMAND_BACKWARD      'B'
#define BLUETOOTH_COMMAND_STEER         'S'
#define BLUETOOTH_COMMAND_MODE          'M'

int *bluetooth_modePointer;        // points to the runningMode value in auto.ino. Used to switch modes
int bluetooth_command = 0;         // stores current command
int bluetooth_commandValue = 0;    // stores current parameter

/**
 * Initializes the bluetooth module
 * @param mode int - address of the runningMode value
 */
void bluetooth_init(int mode)
{
  Logger::info("Initializing bluetooth module");
  bluetooth_modePointer = mode;
}

/**
 * Execute the current command which is stored in bluetooth_command
 */
void bluetooth_execute()
{
  switch(bluetooth_command){
    case BLUETOOTH_COMMAND_FORWARD:
      Logger::info("Going forward");
      motor_controlMotor(MOTOR_FORWARD, bluetooth_commandValue);
      break;
    case BLUETOOTH_COMMAND_BACKWARD:
      Logger::info("Going backward");
      motor_controlMotor(MOTOR_BACKWARD, bluetooth_commandValue);
      break;
    case BLUETOOTH_COMMAND_STEER:
      Logger::info("Steering");
      motor_controlServo(bluetooth_commandValue);
      break;
    case BLUETOOTH_COMMAND_MODE:
      if(bluetooth_commandValue == 0)
      {
        Logger::info("Set mode: tracker");
        *bluetooth_modePointer = 0;
      }else if(bluetooth_commandValue == 1)
      {
        *bluetooth_modePointer = 1;
        Logger::info("Set mode: remote control");
      }
      break;
    default:
      Logger::warning("Command not implemented");
  }
}

/**
 * Call this function every loop. Checks if data is available on the Serial1 ports (bluetooth)
 * Executes command if newline character is read
 */
void bluetooth_loop()
{
  while(Serial1.available() >= 1)
  {
    char read = Serial1.read();

    if(read == '\n')
    {
      bluetooth_execute();
      bluetooth_commandValue = 0;
      bluetooth_command = 0;
    }else if(read >= '0' && read <= '9')
    {
      bluetooth_commandValue *= 10;
      bluetooth_commandValue += read - '0';
    }else if(read >= 'A' && read <= 'Z')
    {
      bluetooth_command = read;
    }
  }
}

#endif
