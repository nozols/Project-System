#ifndef SDcard_h
#define SDcard_h

#include "Arduino.h"
#include "Logger.h"
#include <SPI.h>
#include <SD.h>

class SDcard{
public:
  SDcard(uint8_t pinNumber, const char filename[]);
  bool init();
  void log(uint8_t loglevel, char* msg);
private:
  bool _hasInitialized;
  uint8_t _pinNumber;
  char _filename[32];
};

#endif
