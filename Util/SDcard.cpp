#include "Arduino.h"
#include "SDcard.h"
#include "Logger.h"

/**
 * @param uint8_t pinNumber the chip select pin that is connected
 * @param const char filename[] the filename that is used for logging
 */
SDcard::SDcard(uint8_t pinNumber, const char filename[]) : _sd()
{
  _hasInitialized = false;
  strcpy(_filename, filename);
  _pinNumber = pinNumber;
}

/**
 * Initialize the sd module
 * @returns has this module initialized correctly?
 */
bool SDcard::init()
{
  if(!_sd.begin(_pinNumber))
  {
    Logger::error("ERROR STARTING SD DEVICE!");
  }
  else
  {
    Logger::info("Initialized sd reader");
    _hasInitialized = true;
  }

  return _hasInitialized;
}
