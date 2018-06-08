/*
 * @author Niels de Boer
 * @date 15-05-18
 * @description SDcard functions for logger library
 */
#ifndef SDcard_h
#define SDcard_h

#include "Arduino.h"
#include <SPI.h>

#include "SdFat/src/SdFat.h"

class SDcard{
  public:
    SDcard(uint8_t pinNumber, const char filename[]);
    bool init();
    template<typename T> void log(char* prefix, T msg);
  private:
    bool _hasInitialized;
    uint8_t _pinNumber;
    char _filename[32];
    template<typename T> void storeLog(char* prefix, T msg);
    SdFat _sd;
};

template<typename T> void SDcard::log(char* prefix, T msg)
{
  if(_hasInitialized)
  {
    File dataFile = _sd.open(_filename, FILE_WRITE);

    if(dataFile)
    {
      dataFile.print(prefix);
      dataFile.println(msg);
      dataFile.close();
    }
    else
    {
      _hasInitialized = false;
      Serial.println("FATAL ERROR: file not readable");
      Serial.println("DISABLED SD LOGGING!");
    }
  }
}

#endif
