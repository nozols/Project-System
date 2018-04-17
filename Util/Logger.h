#ifndef Logger_h
#define Logger_h

#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARNING 2
#define LOG_ERROR 3
#define LOG_DATA 4
#define LOG_OFF 5

#include "Arduino.h"
#include "SDcard.h"

#include "Logger.h"

#define LOGGER_SD_ENABLED false      // DECREASES PERFORMANCE
#define LOGGER_USER_FRIENDLY false   // INCREASES LOOPTIME BY 10 MICROSECONDS
#define LOGGER_BUFFER_SIZE 32

/**
 * Logger class
 * Log data
 */
class Logger{
  public:
    template<typename T> static void log(uint8_t loglevel, T value);
    template<typename T> static void debug(T value);
    template<typename T> static void info(T value);
    template<typename T> static void warning(T value);
    template<typename T> static void error(T value);
    template<typename T> static void data(T value);
    template<typename T> static void logarray(uint8_t loglevel, T& value);
    static void getPrefixString(uint8_t loglevel, char *buffer);
    static void getLevelWord(uint8_t loglevel, char *buffer);
    static void getMillisString(char *buffer);
    static bool init();
    static SDcard _sdcard;
    static uint8_t _loglevel;
    static uint8_t _loglevelSD;
    static bool _dataEnabled;
    static bool _userFriendly;
};

/*
 * Define template methods here. Can't be done in Logger.cpp
 */
template<typename T> void Logger::log(uint8_t loglevel, T value){
  char prefixBuffer[32] = {0};
  Logger::getPrefixString(loglevel, prefixBuffer);

  if(loglevel >= Logger::_loglevel)
  {
    Serial.print(prefixBuffer);

    Serial.println(value);
  }

  #if LOGGER_SD_ENABLED

  if(loglevel >= Logger::_loglevelSD)
  {
    Logger::_sdcard.log(prefixBuffer, value);
  }

  #endif
}

template<typename T> void Logger::debug(T value){
  Logger::log(LOG_DEBUG, value);
}

template<typename T> void Logger::info(T value){
  Logger::log(LOG_INFO, value);
}

template<typename T> void Logger::warning(T value){
  Logger::log(LOG_WARNING, value);
}

template<typename T> void Logger::error(T value){
  Logger::log(LOG_ERROR, value);
}

template<typename T> void Logger::data(T value){
  if(Logger::_dataEnabled)
  {
    Logger::log(LOG_DATA, value);
  }
}

template<typename T> void Logger::logarray(uint8_t loglevel, T& value){
  if(loglevel >= Logger::_loglevel)
  {
    char prefixBuffer[LOGGER_BUFFER_SIZE] = {0};
    Logger::getPrefixString(loglevel, prefixBuffer);
    Serial.print(prefixBuffer);

    uint16_t arraysize = sizeof(T) / sizeof(value[0]);
    Serial.print("{");
    for(uint16_t i = 0; i < arraysize; i++)
    {
      Serial.print(value[i]);
      if(i < arraysize - 1)
      {
        Serial.print(" ");
      }
    }
    Serial.println("}");
  }
}


#endif
