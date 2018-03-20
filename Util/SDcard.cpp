#include "Arduino.h"
#include "SDcard.h"

/**
 * @param uint8_t pinNumber the chip select pin that is connected
 * @param const char filename[] the filename that is used for logging
 */
SDcard::SDcard(uint8_t pinNumber, const char filename[])
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
  if(!SD.begin(_pinNumber))
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

/**
 * Log data to the sdcard
 * @param uint8_t loglevel
 * @param char* message the message
 */
void SDcard::log(uint8_t loglevel, char* message)
{
  if(_hasIntialized)
  {
    File dataFile = SD.open("testfile.txt", FILE_WRITE);

    if(dataFile)
    {
      dataFile.println(message);
      dataFile.close();
    }
    else
    {
      Logger::error("Couldn't load file");
    }
  }
}
